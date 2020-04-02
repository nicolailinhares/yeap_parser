#include <stdio.h>
#include <ctype.h>
#include "json_decode.h"
#include "int_stack.h"

#define ST_OBJ 0
#define ST_ARR 1
#define ST_STR 2
#define ST_INT 3
#define ST_FLOAT 4
#define ST_TRUE 5
#define ST_FALSE 6
#define ST_NULL 7
#define ST_PAIR 8
#define ST_ELEMENT 9
#define ST_VALUE 10
#define ST_ESCAPE 11

typedef ERR (*parse_function)(char c, int_stack *states);
ERR pick_parse_function(int_stack *states, parse_function *func);
ERR init_parse(char c, int_stack *states);
ERR obj_parse(char c, int_stack *states);
ERR arr_parse(char c, int_stack *states);
ERR str_parse(char c, int_stack *states);
ERR int_parse(char c, int_stack *states);
ERR float_parse(char c, int_stack *states);
ERR pair_parse(char c, int_stack *states);
ERR value_parse(char c, int_stack *states);
ERR escape_parse(char c, int_stack *states);
ERR by_pass_parse(char c, int_stack *states);
ERR pop_until(int state, int_stack *states);

ERR validate_json(const char *json_text, const int length) {
  int_stack *states = alloc_stack();
  push(states, ST_VALUE);

  for(int i = 0; i < length; i++) {
    int err = 0;
    char c = json_text[i];

    if(c == '}') {
      err = pop_until(ST_OBJ, states);
      err = obj_parse(c, states);
    } else if(c == ',') {
      err = pop_until(ST_PAIR, states);
      err = pair_parse(c, states);
    } else {
      parse_function func;
      pick_parse_function(states, &func);
      err = func(c, states);
    }

    if(err < 0) {
      return err;
    }
  }

  pop_until(ST_VALUE, states);
  pop(states);

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
    case ST_TRUE:
      *func = &by_pass_parse;
      break;
    case ST_FALSE:
      *func = &by_pass_parse;
      break;
    case ST_NULL:
      *func = &by_pass_parse;
      break;
    case ST_PAIR:
      *func = &pair_parse;
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
    printf("\n}\n");
  } else if(c == '"') {
    push(states, ST_PAIR);
    push(states, ST_STR);
  } else if(!isspace(c)) {
    err = ERR_EXPECTING_EOB_OR_PAIR;
  }
  return err;
}

ERR arr_parse(char c, int_stack *states) {
  if(c == ']') {
    pop(states);
  }
  return 0;
}

ERR str_parse(char c, int_stack *states) {
  if(c == '"') {
    pop(states);
    if(is_under(states, ST_VALUE, ST_PAIR)) {
      printf("\"");
    }
  } else if (c == '\\') {
    push(states, ST_ESCAPE);
  } else {
    printf("%c", c);
  }
  return 0;
}

ERR int_parse(char c, int_stack *states) {
  if(isdigit(c)) {
    printf("%c", c);
  } else if (c == '.') {
    pop(states);
    push(states, ST_FLOAT);
    printf(".");
  }
  return 0;
}

ERR float_parse(char c, int_stack *states) {
  if (c == '.') {
    return -1;
  }
  return int_parse(c, states);
}

ERR pair_parse(char c, int_stack *states) {
  if (c == ',') {
    pop(states);
    printf("\n");
  } else if (c == ':') {
    push(states, ST_VALUE);
    printf(":");
  }
  return 0;
}

ERR value_parse(char c, int_stack *states) {
  int err;
  printf("%c", c);
  switch(c) {
    case '"':
      push(states, ST_STR);
      break;
    case '{':
      push(states, ST_OBJ);
      printf("\n");
      break;
    case '[':
      push(states, ST_ARR);
      break;
    case 'n':
      push(states, ST_NULL);
      break;
    case 't':
      push(states, ST_TRUE);
      break;
    case 'f':
      push(states, ST_FALSE);
      break;
    default:
      if (isdigit(c)) {
        push(states, ST_INT);
      } else if (!isspace(c)) {
        return ERR_EXPECTING_VALUE;
      }
  }
  return 0;
}

ERR escape_parse(char c, int_stack *states) {
  if(c == '"' || c == '\\') {
    printf("%c", c);
  } else {
    return ERR_UNEXPECTED_ESCAPED_CHAR;
  }
  return 0;
}

ERR by_pass_parse(char c, int_stack *states) {
  return 0;
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