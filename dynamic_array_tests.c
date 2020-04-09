#include<string.h>
#include<stdio.h>
#include "dynamic_array.h"
#include "unit_test.h"

void dynamic_array_case_1(void) {
  //[1, 1.8, "string"]
  array* arr = alloc_array(AUTO_CAPACITY);
  int value0 = 1;
  double value1 = 1.8;
  char* value2 = "string";
  array_push(arr, (void*) &value0);
  array_push(arr, (void*) &value1);
  array_push(arr, (void*) value2);
  double *ref;
  array_get(arr, 1, (void **) &ref);
  assert(value1 == *ref);
  free_array(arr);
}

void dynamic_array_case_2(void) {
  //[1, 1.8, "string"]
  array* arr = alloc_array(2);
  int value0 = 1;
  double value1 = 1.8;
  char* value2 = "string";
  array_push(arr, (void*) &value0);
  array_push(arr, (void*) &value1);
  array_push(arr, (void*) value2);
  assert(arr->length == 3);
  assert(arr->capacity == 4);
  free_array(arr);
}

void dynamic_array_case_3(void) {
  //[1, 1.8, "string"]
  array* arr = alloc_array(2);
  int value0 = 1;
  double value1 = 1.8;
  char* value2 = "string";
  array_push(arr, (void*) &value0);
  array_push(arr, (void*) &value1);
  array_push(arr, (void*) value2);
  assert(arr->capacity == 4);
  array_pop(arr);
  array_pop(arr);
  assert(arr->capacity == 2);
  free_array(arr);
}

void dynamic_array_case_4(void) {
  //[1, 1.8, "string"]
  array* arr = alloc_array(2);
  int value0 = 1;
  double value1 = 1.8;
  char* value2 = "string";
  array_push(arr, (void*) &value0);
  array_push(arr, (void*) &value1);
  array_push(arr, (void*) value2);
  double *ref;
  int err = array_get(arr, 10, (void **) &ref);
  assert(err == ERR_OUT_OF_BOUNDS);
  assert(ref == NULL);
  free_array(arr);
}

void dynamic_array_case_5(void) {
  array* arr = alloc_array(AUTO_CAPACITY);
  int value = 1;
  array_set(arr, 22, (void*) &value);
  assert(arr->length == 23);
  assert(arr->capacity == 23);
  int *ref;
  array_get(arr, 22, (void **) &ref);
  assert(value == *ref);
  free_array(arr);
}

#define CASES_LENGTH 5

int dynamic_array_tests(void) {
  case_test cases[CASES_LENGTH] = { &dynamic_array_case_1, &dynamic_array_case_2, &dynamic_array_case_3,
                                    &dynamic_array_case_4, &dynamic_array_case_5 };
  run_tests(cases, CASES_LENGTH);

  return 0;
}