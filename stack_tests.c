#include "int_stack.h"
#include "unit_test.h"

void stack_case_1(void) {
  int_stack *stack = alloc_stack();

  push(stack, 18);
  push(stack, -78);

  int top_value;
  top(stack, &top_value);
  assert(top_value == -78);

  free_stack(stack);
}

void stack_case_2(void) {
  int_stack *stack = alloc_stack();

  push(stack, 18);
  push(stack, -78);
  pop(stack);

  int top_value;
  top(stack, &top_value);
  assert(top_value == 18);

  free_stack(stack);
}

void stack_case_3(void) {
  int_stack *stack = alloc_stack();

  assert(true == is_empty(stack));
  push(stack, 18);
  push(stack, -78);
  pop(stack);
  assert(false == is_empty(stack));
  pop(stack);
  assert(true == is_empty(stack));

  free_stack(stack);
}

void stack_case_4(void) {
  int_stack *stack = alloc_stack();

  push(stack, 18);
  push(stack, -78);
  push(stack, -88);
  push(stack, 34);
  push(stack, 10);

  assert(true == is_under(stack, -78, 18));
  assert(false == is_under(stack, -78, -88));
  assert(true == is_under(stack, -78, -78));
  assert(false == is_under(stack, 0, 0));

  free_stack(stack);
}

#define CASES_LENGTH 4

int stack_tests(void) {
  case_test cases[CASES_LENGTH] = {&stack_case_1, &stack_case_2, &stack_case_3, &stack_case_4};
  run_tests(cases, CASES_LENGTH);

  return 0;
}