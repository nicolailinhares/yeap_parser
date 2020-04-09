#include <stdlib.h>
#include "int_stack.h"

#define deref_array_value(type_t, arr, i) *((type_t*) arr->data[i])

int_stack* int_stack_alloc(void) {
  int_stack *stack = malloc(sizeof(int_stack));
  stack->data = alloc_array(AUTO_CAPACITY);
  stack->top_index = -1;
  return stack;
}

ERR int_stack_push(int_stack *stack, int value){
  int *v = malloc(sizeof(int));
  *v = value;
  array_push(stack->data, (void*) v);
  stack->top_index = stack->data->length - 1;
  return 0;
}

ERR int_stack_pop(int_stack *stack) {
  void *v;
  array_top(stack->data, &v);
  free(v);
  array_pop(stack->data);
  stack->top_index = stack->data->length - 1;
  return 0;
}

ERR int_stack_top(int_stack *stack, int *value) {
  if(stack->top_index < 0) {
    return -1;
  }

  int *v;
  array_top(stack->data, (void**) &v);
  *value = *v;

  return 0;
}

bool int_stack_is_empty(int_stack *stack) {
  return stack->top_index < 0;
}

bool int_stack_is_under(int_stack *stack, int value, int limit) {
  for(int i = stack->top_index - 1; i >= 0; i--) {
    if (value == deref_array_value(int, stack->data, i)) {
      return true;
    }

    if (limit == deref_array_value(int, stack->data, i)) {
      return false;
    }
  }

  return false;
}

bool int_stack_is_immediately_under(int_stack *stack, int value) {
  return stack->top_index > 0 &&
          value == deref_array_value(int, stack->data, stack->top_index - 1);
}

ERR int_stack_free(int_stack *stack) {
  for (int i = 0; i < stack->data->length; i++) {
    free(stack->data->data[i]);
  }
  free(stack);

  return 0;
}