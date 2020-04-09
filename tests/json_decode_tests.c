#include <string.h>
#include <stdio.h>
#include "../lib/json_decode.h"
#include "unit_test.h"

void json_case_1(void) {
  char *json_text = "12";
  json_node *json = alloc_json_node();
  int err = json_decode(json_text, strlen(json_text), json);
  int v;
  json_int_value(json, &v);
  assert(12 == v);
  free_json_node(json);
}

void json_case_2(void) {
  char *json_text = "12.87";
  json_node *json = alloc_json_node();
  int err = json_decode(json_text, strlen(json_text), json);
  double v;
  json_double_value(json, &v);
  assert(12.87 == v);
  free_json_node(json);
}

void json_case_3(void) {
  char *json_text = "\"texto\"";
  json_node *json = alloc_json_node();
  int err = json_decode(json_text, strlen(json_text), json);
  char *v;
  json_string_value(json, &v);
  assert(strcmp("texto", v) == 0);
  free_json_node(json);
}

void json_case_4(void) {
  char *json_text = "null";
  json_node *json = alloc_json_node();
  int err = json_decode(json_text, strlen(json_text), json);
  char *v;
  json_string_value(json, &v);
  assert(NULL == v);
  free_json_node(json);
}

void json_case_5(void) {
  char *json_text = "false";
  json_node *json = alloc_json_node();
  int err = json_decode(json_text, strlen(json_text), json);
  bool v;
  json_bool_value(json, &v);
  assert(false == v);
  free_json_node(json);
}

void json_case_6(void) {
  char *json_text = "true";
  json_node *json = alloc_json_node();
  int err = json_decode(json_text, strlen(json_text), json);
  bool v;
  json_bool_value(json, &v);
  assert(true == v);
  free_json_node(json);
}

void json_case_7(void) {
  char *json_text = "[1, 1.8, \"jon\", null, true, false, 10]";
  json_node *json = alloc_json_node();
  int err = json_decode(json_text, strlen(json_text), json);
  array *v;
  json_array_value(json, &v);
  void *e;
  array_get(v, 4, &e);
  assert(true == *((bool*)e));
  array_get(v, 2, &e);
  assert(strcmp("jon", (char*) e) == 0);
  free_json_node(json);
}

#define CASES_LENGTH 7

int json_decode_tests(void) {
  case_test cases[CASES_LENGTH] = { &json_case_1, &json_case_2, &json_case_3,
                                    &json_case_4, &json_case_5, &json_case_6,
                                    &json_case_7 };
  run_tests(cases, CASES_LENGTH);

  return 0;
}