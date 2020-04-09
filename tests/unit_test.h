#ifndef LINHARES_UNIT_SUIT_H
#define LINHARES_UNIT_SUIT_H

#include <assert.h>

typedef void (*case_test)(void);
void run_tests(case_test test_suite[], int n);

#endif