#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "json_decode.h"
#include "int_stack.h"

#define ST_OBJ 0
#define ST_ARR 1
#define ST_STR 2
#define ST_INT 3
#define ST_FLOAT 4
#define ST_SPECIAL_VALUE 5
#define ST_PAIR 8
#define ST_ELEMENT 9
#define ST_VALUE 10
#define ST_ESCAPE 11

typedef struct _pair {
  char* key;
  void* value;
} pair;

typedef ERR (*parse_function)(char c, int_stack *states);
ERR pick_parse_function(int_stack *states, parse_function *func);
ERR init_parse(char c, int_stack *states);
ERR obj_parse(char c, int_stack *states);
ERR arr_parse(char c, int_stack *states);
ERR str_parse(char c, int_stack *states);
ERR int_parse(char c, int_stack *states);
ERR float_parse(char c, int_stack *states);
ERR special_value_parse(char c, int_stack *states);
ERR pair_parse(char c, int_stack *states);
ERR element_parse(char c, int_stack *states);
ERR value_parse(char c, int_stack *states);
ERR escape_parse(char c, int_stack *states);
ERR pop_until(int state, int_stack *states);
ERR parse_afert_pop(char c, int state, int_stack *states);

void print_top(int_stack *states){
  int state;
  top(states, &state);
  printf("----STATE %i: %i------", states->top_index, state);
}

ERR validate_json(const char *json_text, const int length) {
  int_stack *states = alloc_stack();
  push(states, ST_VALUE);
  parse_function func;
  int err = ERR_OK;

  for(int i = 0; i < length; i++) {
    char c = json_text[i];

    pick_parse_function(states, &func);
    err = func(c, states);

    if(err < 0) {
      return err;
    }
  }

  pick_parse_function(states, &func);
  err = func(EOF, states);

  if(err < 0) {
    return err;
  }

  return is_empty(states) ? ERR_OK : ERR_UNEXPECTED_EOF;
}

ERR pick_parse_function(int_stack *states, parse_function *func) {
  int state;

  top(states, &state);
  switch(state) {
    case ST_OBJ:
      *func = &obj_parse;
      break;
    case ST_ARR:
      *func = &arr_parse;
      break;
    case ST_STR:
      *func = &str_parse;
      break;
    case ST_INT:
      *func = &int_parse;
      break;
    case ST_FLOAT:
      *func = &float_parse;
      break;
    case ST_SPECIAL_VALUE:
      *func = &special_value_parse;
      break;
    case ST_PAIR:
      *func = &pair_parse;
      break;
    case ST_ELEMENT:
      *func = &element_parse;
      break;
    case ST_VALUE:
      *func = &value_parse;
      break;
    case ST_ESCAPE:
      *func = &escape_parse;
      break;
  }
  return 0;
}

ERR obj_parse(char c, int_stack *states) {
  int err = 0;
  if(c == '}') {
    pop(states);
  } else if(c == '"') {
    push(states, ST_PAIR);
    pair_parse(c, states);
  } else if(c == ',') {
    push(states, ST_PAIR);
  }
   else if(!isspace(c)) {
    err = ERR_EXPECTING_EOB_OR_PAIR;
  }
  return err;
}

ERR arr_parse(char c, int_stack *states) {
  if(c == ']') {
    pop(states);
  } else if (c == ',' || c == '}') {
    return ERR_EXPECTING_ELEMENT;
  } else {
    push(states, ST_ELEMENT);
    push(states, ST_VALUE);
    value_parse(c, states);
  }
  return 0;
}

ERR str_parse(char c, int_stack *states) {
  if(c == '"') {
    pop(states);
  } else if (c == '\\') {
    push(states, ST_ESCAPE);
  }
  return 0;
}

ERR int_parse(char c, int_stack *states) {
  if (c == '.') {
    pop(states);
    push(states, ST_FLOAT);
  } else if (c == ',' || c == '}' || c == ']' || c == EOF) {
    parse_afert_pop(c, ST_VALUE, states);
  }
  return 0;
}

ERR float_parse(char c, int_stack *states) {
  if (c == '.') {
    return ERR_EXPECTING_DIGIT_OR_END;
  }
  return int_parse(c, states);
}

ERR pair_parse(char c, int_stack *states) {
  if (c == '"') {
    push(states, ST_STR);
  } else if (c == ':') {
    push(states, ST_VALUE);
  } else if (c == '}') {
    return ERR_EXPECTING_PAIR;
  }
  return 0;
}

ERR element_parse(char c, int_stack *states) {
  if (c == '}') {
    pop(states);
  } else if (c == ']') {
    return ERR_EXPECTING_ELEMENT;
  } else if (c != ',') {
    push(states, ST_VALUE);
    value_parse(c, states);
  }
  return 0;
}

ERR value_parse(char c, int_stack *states) {
  switch(c) {
    case '"':
      push(states, ST_STR);
      break;
    case '{':
      push(states, ST_OBJ);
      break;
    case '[':
      push(states, ST_ARR);
      break;
    case ',':
      if(is_immediately_under(states, ST_PAIR)) {
        parse_afert_pop(c, ST_OBJ, states);
      }
      if (is_immediately_under(states, ST_ELEMENT)) {
        parse_afert_pop(c, ST_ELEMENT, states);
      }
      break;
    case '}':
      parse_afert_pop(c, ST_OBJ, states);
      if (is_immediately_under(states, ST_ELEMENT)) {
        parse_afert_pop(c, ST_ELEMENT, states);
      }
      break;
    case ']':
      parse_afert_pop(c, ST_ARR, states);
      break;
    case EOF:
      pop(states);
      break;
    default:
      if (isdigit(c)) {
        push(states, ST_INT);
      } else if(c == 'n' || c == 'f' || c == 't') {
        push(states, ST_SPECIAL_VALUE);
        special_value_parse(c, states);
      } else if (!isspace(c)) {
        return ERR_EXPECTING_VALUE;
      }
  }
  return 0;
}

ERR escape_parse(char c, int_stack *states) {
  if(c == '"' || c == '\\') {
    pop(states);
  } else {
    return ERR_UNEXPECTED_ESCAPED_CHAR;
  }
  return 0;
}

ERR special_value_parse(char c, int_stack *states) {
  static int idx = 0;
  static char value[6] = "";
  int err = ERR_OK;

  if (c == ',' || c == ']' || c == '}' || c == EOF) {
    char ref[6] = "";
    value[idx] = '\0';

    switch(value[0]) {
      case 't':
        strcpy(ref, "true");
        break;
      case 'f':
        strcpy(ref, "false");
        break;
      case 'n':
        strcpy(ref, "null");
        break;
    }
    if (strcmp(ref, value) != 0) {
      err = ERR_EXPECTING_VALUE;
    } else {
      err = parse_afert_pop(c, ST_VALUE, states);
    }
    idx = 0;
  } else if (idx < 5) {
    value[idx] = c;
    idx++;
  } else {
    err = ERR_EXPECTING_VALUE;
    idx = 0;
  }

  return err;
}

ERR pop_until(int state, int_stack *states) {
  int curr_state;
  top(states, &curr_state);
  while (curr_state != state) {
    pop(states);
    if(is_empty(states)) {
      return ERR_ANCESTOR_NOT_FOUND;
    }
    top(states, &curr_state);
  }
  return 0;
}

ERR parse_afert_pop(char c, int state, int_stack *states) {
  pop_until(state, states);

  parse_function func;
  pick_parse_function(states, &func);
  return func(c, states);
}