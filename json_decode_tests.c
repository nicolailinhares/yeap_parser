#include<string.h>
#include<stdio.h>
#include "json_decode.h"
#include "unit_test.h"

void json_case_1(void) {
  char *json = "{\"age\": 46, \"name\": \"jon\", \"balance\": 415.84}";
  int err = validate_json(json, strlen(json));
  assert(0 == err);
}

void json_case_2(void) {
  char *json = "{\"age\": 12, \"name\": \"jon\", \"balance\": 415.84";
  assert(ERR_UNEXPECTED_EOF == validate_json(json, strlen(json)));
}

void json_case_3(void) {
  char *json = "{ age\": 12, \"name\": \"jon\", \"balance\": 415.84}";
  assert(ERR_EXPECTING_EOB_OR_PAIR == validate_json(json, strlen(json)));
}

void json_case_4(void) {
  char *json = "{\"age\": 46, \"name\": \"jon\", \"balance\": 415.84,}";
  int err = validate_json(json, strlen(json));
  printf(" skipped ");
  //assert(-1 == err);
}

void json_case_5(void) {
  char *json = "12";
  int err = validate_json(json, strlen(json));
  assert(0 == err);
}

void json_case_6(void) {
  char *json = "47.89";
  int err = validate_json(json, strlen(json));
  assert(0 == err);
}

void json_case_7(void) {
  char *json = "\"jon\"";
  int err = validate_json(json, strlen(json));
  assert(0 == err);
}

#define CASES_LENGTH 7

int json_decode_tests(void) {
  case_test cases[CASES_LENGTH] = {&json_case_1, &json_case_2, &json_case_3, &json_case_4, &json_case_5, &json_case_6, &json_case_7};
  run_tests(cases, CASES_LENGTH);

  return 0;
}