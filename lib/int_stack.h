#ifndef LINHARES_INT_STACK_H
#define LINHARES_INT_STACK_H

#include <stdbool.h>
#include "stack.h"

typedef int ERR;
typedef stack int_stack;

int_stack* int_stack_alloc(void);
ERR int_stack_push(int_stack *stack, int value);
ERR int_stack_pop(int_stack *stack);
ERR int_stack_top(int_stack *stack, int *value);
bool int_stack_is_empty(int_stack *stack);
bool int_stack_is_under(int_stack *stack, int value, int limit);
bool int_stack_is_immediately_under(int_stack *stack, int value);
ERR int_stack_free(int_stack *stack);

#endif