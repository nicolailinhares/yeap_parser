#ifndef LINHARES_INT_STACK_H
#define LINHARES_INT_STACK_H

#include <stdbool.h>
#define STACK_SIZE 10000

typedef int ERR;
typedef struct _int_stack {
  int top_index;
  int stack[STACK_SIZE];
} int_stack;

int_stack* alloc_stack(void);
ERR push(int_stack *stack, int value);
ERR pop(int_stack *stack);
ERR top(int_stack *stack, int *value);
bool is_empty(int_stack *stack);
bool is_under(int_stack *stack, int value, int limit);
ERR free_stack(int_stack *stack);

#endif