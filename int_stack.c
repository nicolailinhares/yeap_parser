#include <stdlib.h>

#include "int_stack.h"

int_stack* alloc_stack(void) {
  int_stack *stack = malloc(sizeof(int_stack));
  stack->top_index = -1;
  return stack;
}

ERR push(int_stack *stack, int value){
  stack->stack[++stack->top_index] = value;
  return 0;
}

ERR pop(int_stack *stack) {
  stack->top_index--;
  return 0;
}

ERR top(int_stack *stack, int *value) {
  if(stack->top_index < 0) {
    return -1;
  }

  *value = stack->stack[stack->top_index];
  return 0;
}

bool is_empty(int_stack *stack) {
  return stack->top_index < 0;
}

bool is_under(int_stack *stack, int value, int limit) {
  for(int i = stack->top_index; i >= 0; i--) {
    if (value == stack->stack[i]) {
      return true;
    }

    if (limit == stack->stack[i]) {
      return false;
    }
  }

  return false;
}

ERR free_stack(int_stack *stack) {
  free(stack);

  return 0;
}