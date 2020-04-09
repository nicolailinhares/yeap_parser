#include "int_stack.h"
#include "unit_test.h"

void stack_case_1(void) {
  int_stack *stack = int_stack_alloc();

  int_stack_push(stack, 18);
  int_stack_push(stack, -78);

  int top_value;
  int_stack_top(stack, &top_value);
  assert(top_value == -78);

  int_stack_free(stack);
}

void stack_case_2(void) {
  int_stack *stack = int_stack_alloc();

  int_stack_push(stack, 18);
  int_stack_push(stack, -78);
  int_stack_pop(stack);

  int top_value;
  int_stack_top(stack, &top_value);
  assert(top_value == 18);

  int_stack_free(stack);
}

void stack_case_3(void) {
  int_stack *stack = int_stack_alloc();

  assert(true == int_stack_is_empty(stack));
  int_stack_push(stack, 18);
  int_stack_push(stack, -78);
  int_stack_pop(stack);
  assert(false == int_stack_is_empty(stack));
  int_stack_pop(stack);
  assert(true == int_stack_is_empty(stack));

  int_stack_free(stack);
}

void stack_case_4(void) {
  int_stack *stack = int_stack_alloc();

  int_stack_push(stack, 18);
  int_stack_push(stack, -78);
  int_stack_push(stack, -88);
  int_stack_push(stack, 34);
  int_stack_push(stack, 10);

  assert(true == int_stack_is_under(stack, -78, 18));
  assert(false == int_stack_is_under(stack, -78, -88));
  assert(true == int_stack_is_under(stack, -78, -78));
  assert(false == int_stack_is_under(stack, 0, 0));

  int_stack_free(stack);
}

void stack_case_5(void) {
  int_stack *stack = int_stack_alloc();

  int_stack_push(stack, 18);
  int_stack_push(stack, -78);
  int_stack_push(stack, -88);
  int_stack_push(stack, 34);
  int_stack_push(stack, 10);

  assert(false == int_stack_is_immediately_under(stack, -88));
  int_stack_pop(stack);
  assert(true == int_stack_is_immediately_under(stack, -88));

  int_stack_free(stack);
}

#define CASES_LENGTH 5

int stack_tests(void) {
  case_test cases[CASES_LENGTH] = {&stack_case_1, &stack_case_2, &stack_case_3, &stack_case_4, &stack_case_5};
  run_tests(cases, CASES_LENGTH);

  return 0;
}