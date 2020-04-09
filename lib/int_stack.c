#include <stdlib.h>
#include "int_stack.h"
#include "stack.h"

#define deref_array_value(type_t, arr, i) *((type_t*) arr->data[i])

int_stack* int_stack_alloc(void) {
  return stack_alloc();
}

ERR int_stack_push(int_stack *stack, int value){
  int *v = malloc(sizeof(int));
  *v = value;
  stack_push(stack, (void*) v);
  return 0;
}

ERR int_stack_pop(int_stack *stack) {
  stack_pop(stack);
  return 0;
}

ERR int_stack_top(int_stack *stack, int *value) {
  int *v = NULL;
  int err = stack_top(stack, (void**) &v);
  if(err < 0) {
      return -1;
  }
  *value = *v;
  return 0;
}

bool int_stack_is_empty(int_stack *stack) {
  return stack_is_empty(stack);
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
  stack_free(stack);

  return 0;
}