#include <stdlib.h>
#include "stack.h"

stack* stack_alloc(void) {
  stack *stack = malloc(sizeof(stack));
  stack->data = alloc_array(AUTO_CAPACITY);
  stack->top_index = -1;
  return stack;
}

ERR stack_push(stack *stack, void *value){
  array_push(stack->data, value);
  stack->top_index = stack->data->length - 1;
  return 0;
}

ERR stack_pop(stack *stack) {
  void *v;
  array_top(stack->data, &v);
  free(v);
  array_pop(stack->data);
  stack->top_index = stack->data->length - 1;
  return 0;
}

ERR stack_top(stack *stack, void **value) {
  if(stack->top_index < 0) {
    return -1;
  }

  array_top(stack->data, value);

  return 0;
}

bool stack_is_empty(stack *stack) {
  return stack->top_index < 0;
}

ERR stack_free(stack *stack) {
  for (int i = 0; i < stack->data->length; i++) {
    free(stack->data->data[i]);
  }
  free(stack);

  return 0;
}