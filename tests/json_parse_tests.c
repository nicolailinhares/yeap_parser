#include <string.h>
#include <stdio.h>
#include "../lib/json_decode.h"
#include "unit_test.h"

void json_parse_case_1(void) {
  char *json = "{\"age\": 46, \"name\": \"jon\", \"balance\": 415.84}";
  int err = validate_json(json, strlen(json));
  assert(0 == err);
}

void json_parse_case_2(void) {
  char *json = "{\"age\": 12, \"name\": \"jon\", \"balance\": 415.84";
  assert(ERR_UNEXPECTED_EOF == validate_json(json, strlen(json)));
}

void json_parse_case_3(void) {
  char *json = "{ age\": 12, \"name\": \"jon\", \"balance\": 415.84}";
  assert(ERR_EXPECTING_EOB_OR_PAIR == validate_json(json, strlen(json)));
}

void json_parse_case_4(void) {
  char *json = "{\"age\": 46, \"name\": \"jon\", \"balance\": 415.84,}";
  int err = validate_json(json, strlen(json));
  assert(ERR_EXPECTING_PAIR == err);
}

void json_parse_case_5(void) {
  char *json = "12";
  int err = validate_json(json, strlen(json));
  assert(0 == err);
}

void json_parse_case_6(void) {
  char *json = "47.89";
  int err = validate_json(json, strlen(json));
  assert(0 == err);
}

void json_parse_case_7(void) {
  char *json = "\"jon(\\\"great\\\\\\\")\"";
  int err = validate_json(json, strlen(json));
  assert(0 == err);
}

void json_parse_case_8(void) {
  char *json = "[1, 1.8, \"jon\", [1 , 2, 3], {\"age\": 46, \"name\": \"jon\", \"balance\": 415.84}]";
  int err = validate_json(json, strlen(json));
  assert(0 == err);
}

void json_parse_case_9(void) {
  char *json = "{\"age\": 46, \"name\": \"jon\", \"balance\": 415.84, \"jon\": {\"age\": 46, \"name\": \"jon\", \"balance\": 415.84, \"jon\": {\"age\": 46, \"name\": \"jon\", \"balance\": 415.84}} }";
  int err = validate_json(json, strlen(json));
  assert(0 == err);
}

void json_parse_case_10(void) {
  char *json = "\"a\\ge\"";
  int err = validate_json(json, strlen(json));
  assert(ERR_UNEXPECTED_ESCAPED_CHAR == err);
}

void json_parse_case_11(void) {
  char *json = "null";
  int err = validate_json(json, strlen(json));
  assert(ERR_OK == err);
}

void json_parse_case_12(void) {
  char *json = "true";
  int err = validate_json(json, strlen(json));
  assert(ERR_OK == err);
}

void json_parse_case_13(void) {
  char *json = "false";
  int err = validate_json(json, strlen(json));
  assert(ERR_OK == err);
}

void json_parse_case_14(void) {
  char *json = "{\"age\": null, \"name\": false, \"balance\": true, \"values\": [null, 1, false, \"str\", 78.65, true]}";
  int err = validate_json(json, strlen(json));
  assert(ERR_OK == err);
}

void json_parse_case_15(void) {
  char *json = "nll";
  int err = validate_json(json, strlen(json));
  assert(ERR_EXPECTING_VALUE == err);
}

void json_parse_case_16(void) {
  char *json = "truel";
  int err = validate_json(json, strlen(json));
  assert(ERR_EXPECTING_VALUE == err);
}

void json_parse_case_17(void) {
  char *json = "falselong";
  int err = validate_json(json, strlen(json));
  assert(ERR_EXPECTING_VALUE == err);
}

void json_parse_case_18(void) {
  char *json = "[1, 2, 3,]";
  int err = validate_json(json, strlen(json));
  assert(ERR_EXPECTING_ELEMENT == err);
}

void json_parse_case_19(void) {
  char *json = "[, 1, 2, 3]";
  int err = validate_json(json, strlen(json));
  assert(ERR_EXPECTING_ELEMENT == err);
}

void json_parse_case_20(void) {
  char *json = "48..7";
  int err = validate_json(json, strlen(json));
  assert(ERR_EXPECTING_DIGIT_OR_END == err);
}

#define CASES_LENGTH 20

int json_parse_tests(void) {
  case_test cases[CASES_LENGTH] = { &json_parse_case_1, &json_parse_case_2, &json_parse_case_3,
                                    &json_parse_case_4, &json_parse_case_5, &json_parse_case_6,
                                    &json_parse_case_7, &json_parse_case_8, &json_parse_case_9,
                                    &json_parse_case_10, &json_parse_case_11, &json_parse_case_12,
                                    &json_parse_case_13, &json_parse_case_14, &json_parse_case_15,
                                    &json_parse_case_16, &json_parse_case_17, &json_parse_case_18,
                                    &json_parse_case_19, &json_parse_case_20 };
  run_tests(cases, CASES_LENGTH);

  return 0;
}