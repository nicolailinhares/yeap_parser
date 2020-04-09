#include <stdio.h>
#include "unit_test.h"

void run_test(case_test test) {
  test();
  printf(".");
}

void run_tests(case_test test_suite[], int n){
  for (int i = 0; i < n; i++) {
    run_test(test_suite[i]);
  }
  printf("\n");
}