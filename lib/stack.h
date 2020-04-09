#ifndef YETAPARSER_STACK_H
#define YETAPARSER_STACK_H

#include <stdbool.h>
#include "dynamic_array.h"

typedef int ERR;
typedef struct _stack {
  int top_index;
  array *data;
} stack;

stack* stack_alloc(void);
ERR stack_push(stack *stack, void *value);
ERR stack_pop(stack *stack);
ERR stack_top(stack *stack, void **value);
bool stack_is_empty(stack *stack);
ERR stack_free(stack *stack);

#endif