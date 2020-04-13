#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "json_decode.h"
#include "int_stack.h"
#include "stack.h"
#include "dict.h"
#include "dynamic_array.h"

#define MAX_KEY_LENGTH 45

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
  char key[MAX_KEY_LENGTH];
  void* value;
} pair;

typedef struct _context {
  int_stack *states;
  stack *data_context;
  char current_token[10000];
  int current_index;
  void *current_value;
} decode_context;

typedef ERR (*parse_function)(char c, decode_context *context);
static ERR decoding_machine(const char* json_text, const int length, decode_context *context);
static ERR context_push(decode_context *context, int state);
static ERR context_add(decode_context *context, char c);
static void context_reset(decode_context *context);
static ERR context_transfer_element(decode_context *context);
static ERR context_transfer_key(decode_context *context);
static ERR context_transfer_pair_value(decode_context *context);
static ERR context_transfer_pair(decode_context *context);
static ERR context_pop_data(decode_context *context);
static ERR context_close(decode_context *context);
static ERR pick_parse_function(decode_context *context, parse_function *func);
static ERR obj_parse(char c, decode_context *context);
static ERR arr_parse(char c, decode_context *context);
static ERR str_parse(char c, decode_context *context);
static ERR int_parse(char c, decode_context *context);
static ERR float_parse(char c, decode_context *context);
static ERR special_value_parse(char c, decode_context *context);
static ERR pair_parse(char c, decode_context *context);
static ERR element_parse(char c, decode_context *context);
static ERR value_parse(char c, decode_context *context);
static ERR escape_parse(char c, decode_context *context);
static ERR pop_until(int state, int_stack *states);
static ERR parse_afert_pop(char c, int state, decode_context *context);
static decode_context* alloc_context(bool has_data);
static void free_context(decode_context *context);

void print_top(int_stack *states){
  int state;
  int_stack_top(states, &state);
  printf("----STATE %i: %i------", states->top_index, state);
}

ERR validate_json(const char *json_text, const int length) {
  decode_context *context = alloc_context(false);

  int err = decoding_machine(json_text, length, context);

  free_context(context);
  return err;
}

ERR json_decode(const char *json_text, const int length, json_node *json) {
  decode_context *context = alloc_context(true);

  int err = decoding_machine(json_text, length, context);
  if (err == ERR_OK) {
    strcpy(json->key, "((root))");
    stack_top(context->data_context, &(json->value));
    stack_pop(context->data_context);
    err = stack_is_empty(context->data_context) ? ERR_OK : ERR_UNEXPECTED_EOF;
  }

  free_context(context);
  return err;
}

static ERR decoding_machine(const char *json_text, const int length, decode_context *context) {
  parse_function func;
  int err = ERR_OK;

  int_stack_push(context->states, ST_VALUE);
  for(int i = 0; i < length; i++) {
    char c = json_text[i];

    pick_parse_function(context, &func);
    err = func(c, context);

    if(err < 0) {
      return err;
    }
  }

  pick_parse_function(context, &func);
  err = func(EOF, context);

  if(err < 0) {
    return err;
  }

  bool is_states_empty = int_stack_is_empty(context->states);

  return is_states_empty ? ERR_OK : ERR_UNEXPECTED_EOF;
}

static ERR pick_parse_function(decode_context *context, parse_function *func) {
  int state;

  int_stack_top(context->states, &state);
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

static ERR obj_parse(char c, decode_context *context) {
  int err = 0;
  if(c == '}') {
    context_transfer_pair(context);
    context_pop_data(context);
    int_stack_pop(context->states);
  } else if(c == '"') {
    context_push(context, ST_PAIR);
    pair_parse(c, context);
  } else if(c == ',') {
    context_transfer_pair(context);
    context_push(context, ST_PAIR);
  }
   else if(!isspace(c)) {
    err = ERR_EXPECTING_EOB_OR_PAIR;
  }
  return err;
}

static ERR arr_parse(char c, decode_context *context) {
  if(c == ']') {
    context_transfer_element(context);
    context_pop_data(context);
    int_stack_pop(context->states);
  } else if (c == ',' || c == '}') {
    return ERR_EXPECTING_ELEMENT;
  } else {
    context_push(context, ST_ELEMENT);
    element_parse(c, context);
  }
  return 0;
}

static ERR str_parse(char c, decode_context *context) {
  if(c == '"') {
    context_add(context, '\0');
    context->current_value = malloc(sizeof(char)*(context->current_index + 2));
    strcpy(context->current_value, context->current_token);
    context_reset(context);
    int_stack_pop(context->states);
  } else if (c == '\\') {
    context_push(context, ST_ESCAPE);
  } else {
    context_add(context, c);
  }
  return 0;
}

static ERR int_parse(char c, decode_context *context) {
  if (c == '.') {
    int_stack_pop(context->states);
    context_push(context, ST_FLOAT);
    context_add(context, c);
  } else if (c == ',' || c == '}' || c == ']' || c == EOF) {
    context_add(context, '\0');
    context->current_value = malloc(sizeof(int));
    *((int*) context->current_value) = strtoimax(context->current_token, NULL, 10);
    context_reset(context);
    parse_afert_pop(c, ST_VALUE, context);
  } else {
    context_add(context, c);
  }
  return 0;
}

static ERR float_parse(char c, decode_context *context) {
  if (c == '.') {
    return ERR_EXPECTING_DIGIT_OR_END;
  } else if (c == ',' || c == '}' || c == ']' || c == EOF) {
    context_add(context, '\0');
    context->current_value = malloc(sizeof(double));
    *((double*) context->current_value) = strtod(context->current_token, NULL);
    context_reset(context);
    return parse_afert_pop(c, ST_VALUE, context);
  }
  return int_parse(c, context);
}

static ERR pair_parse(char c, decode_context *context) {
  if (c == '"') {
    context_push(context, ST_STR);
  } else if (c == ':') {
    context_transfer_key(context);
    context_push(context, ST_VALUE);
  } else if (c == '}') {
    return ERR_EXPECTING_PAIR;
  }
  return 0;
}

static ERR element_parse(char c, decode_context *context) {
  if (c == ']') {
    return ERR_EXPECTING_ELEMENT;
  } else if(c == ',') {
    context_transfer_element(context);
  } else {
    context_push(context, ST_VALUE);
    value_parse(c, context);
  }
  return 0;
}

static ERR value_parse(char c, decode_context *context) {
  int err = ERR_OK;
  switch(c) {
    case '"':
      context_push(context, ST_STR);
      break;
    case '{':
      context_push(context, ST_OBJ);
      break;
    case '[':
      context_push(context, ST_ARR);
      break;
    case ',':
      if(int_stack_is_immediately_under(context->states, ST_PAIR)) {
        context_transfer_pair_value(context);
        err = parse_afert_pop(c, ST_OBJ, context);
      }
      if (err == ERR_OK && int_stack_is_immediately_under(context->states, ST_ELEMENT)) {
        err = parse_afert_pop(c, ST_ELEMENT, context);
      }
      break;
    case '}':
      context_transfer_pair_value(context);
      err = parse_afert_pop(c, ST_OBJ, context);
      break;
    case ']':
      err = parse_afert_pop(c, ST_ARR, context);
      break;
    case EOF:
      int_stack_pop(context->states);
      context_close(context);
      break;
    default:
      if (isdigit(c)) {
        context_push(context, ST_INT);
        err = int_parse(c, context);
      } else if(c == 'n' || c == 'f' || c == 't') {
        context_push(context, ST_SPECIAL_VALUE);
        err = special_value_parse(c, context);
      } else if (!isspace(c)) {
        return ERR_EXPECTING_VALUE;
      }
  }
  return err;
}

static ERR escape_parse(char c, decode_context *context) {
  if(c == '"' || c == '\\') {
    int_stack_pop(context->states);
  } else {
    return ERR_UNEXPECTED_ESCAPED_CHAR;
  }
  return 0;
}

static ERR special_value_parse(char c, decode_context *context) {
  int err = ERR_OK;

  if (c == ',' || c == ']' || c == '}' || c == EOF) {
    char ref[6] = "";
    context_add(context, '\0');
    bool *value = malloc(sizeof(bool));
    switch(context->current_token[0]) {
      case 't':
        strcpy(ref, "true");
        *value = true;
        break;
      case 'f':
        strcpy(ref, "false");
        *value = false;
        break;
      case 'n':
        strcpy(ref, "null");
        free(value);
        value = NULL;
        break;
    }
    if (strcmp(ref, context->current_token) != 0) {
      err = ERR_EXPECTING_VALUE;
    } else {
      context->current_value = value;
      err = parse_afert_pop(c, ST_VALUE, context);
    }
    context_reset(context);
  } else if (context->current_index < 5) {
    context_add(context, c);
  } else {
    err = ERR_EXPECTING_VALUE;
    context_reset(context);
  }

  return err;
}

static ERR context_push(decode_context *context, int state) {
  int err = int_stack_push(context->states, state);
  if (err == ERR_OK && context->data_context != NULL) {
    switch(state) {
      case ST_OBJ:
        err = stack_push(context->data_context, (void**) alloc_dict());
        break;
      case ST_PAIR:
        err = stack_push(context->data_context, malloc(sizeof(pair)));
        break;
      case ST_ARR:
        err = stack_push(context->data_context, (void**) alloc_array(AUTO_CAPACITY));
        break;
    }
  }

  return err == ERR_OK ? ERR_OK : ERR_COULD_NOT_PUSH_CONTEXT;
}

static ERR context_add(decode_context *context, char c) {
  context->current_token[context->current_index++] = c;
  return 0;
}

static void context_reset(decode_context *context) {
  context->current_token[0] = '\0';
  context->current_index = 0;
}

static ERR context_transfer_element(decode_context *context) {
  if (context->data_context != NULL) {
    array *arr;
    stack_top(context->data_context, (void**) &arr);
    array_push(arr, context->current_value);
  }
  context->current_value = NULL;

  return 0;
}

static ERR context_transfer_key(decode_context *context) {
  if (context->data_context != NULL) {
    pair *p;
    stack_top(context->data_context, (void**) &p);
    strcpy(p->key, context->current_value);
  }
  context->current_value = NULL;

  return 0;
}

static ERR context_transfer_pair_value(decode_context *context) {
  if (context->data_context != NULL) {
    pair *p;
    stack_top(context->data_context, (void**) &p);
    p->value = context->current_value;
  }
  context->current_value = NULL;

  return 0;
}

static ERR context_transfer_pair(decode_context *context) {
  if (context->data_context != NULL) {
    pair *p;
    stack_top(context->data_context, (void**) &p);
    stack_pop(context->data_context);
    dict *r;
    stack_top(context->data_context, (void**) &r);
    insert_ref(r, p->key, p->value);
    free(p);
  }
  context->current_value = NULL;

  return 0;
}

static ERR context_pop_data(decode_context *context) {
  if (context->data_context != NULL) {
    void *data;
    stack_top(context->data_context, &data);
    stack_pop(context->data_context);
    context->current_value = data;
  }

  return 0;
}

static ERR context_close(decode_context *context) {
  if(context->data_context != NULL) {
    stack_push(context->data_context, context->current_value);
  }
  return 0;
}

static ERR pop_until(int state, int_stack *states) {
  int curr_state;
  int_stack_top(states, &curr_state);
  while (curr_state != state) {
    int_stack_pop(states);
    if(int_stack_is_empty(states)) {
      return ERR_ANCESTOR_NOT_FOUND;
    }
    int_stack_top(states, &curr_state);
  }
  return 0;
}

static ERR parse_afert_pop(char c, int state, decode_context *context) {
  pop_until(state, context->states);

  parse_function func;
  pick_parse_function(context, &func);
  return func(c, context);
}

static decode_context* alloc_context(bool has_data) {
  decode_context* context = malloc(sizeof(decode_context));
  context->states = int_stack_alloc();
  context_reset(context);
  context->current_value = NULL;
  if (has_data) {
    context->data_context = stack_alloc();
  } else {
    context->data_context = NULL;
  }

  return context;
}

static void free_context(decode_context *context) {
  int_stack_free(context->states);
  if(context->data_context != NULL) {
    stack_free(context->data_context);
  }
  free(context);
}