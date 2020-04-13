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

void json_case_8(void) {
  char *json_text = "[1, 1.8, \"jon\", null, [28, 38.89], false, 10]";
  json_node *json = alloc_json_node();
  int err = json_decode(json_text, strlen(json_text), json);
  array *v;
  json_array_value(json, &v);
  void *e;
  array_get(v, 4, &e);
  void *sube;
  array_get((array*) e, 1, &sube);
  assert(38.89 == *((double*)sube));
  array_get(v, 2, &e);
  assert(strcmp("jon", (char*) e) == 0);
  free_json_node(json);
}

void json_case_9(void) {
  char *json_text = "{\"age\": 46, \"name\": \"jon\", \"balance\": 415.84}";;
  json_node *json = alloc_json_node();
  int err = json_decode(json_text, strlen(json_text), json);
  dict *v;
  json_dict_value(json, &v);
  int e;
  get_int(v, "age", &e);
  assert(46 == e);
  double de;
  get_double(v, "balance", &de);
  assert(415.84 == de);
  char *se;
  get_ref(v, "name", (void**) &se);
  assert(strcmp("jon", se) == 0);
  free_json_node(json);
}

void json_case_10(void) {
  char *json_text = "{\"age\": 46, \"name\": \"jon\", \"balance\": 415.84, \"statements\": [{\"name\": \"joe\", \"value\": 215.84}, {\"name\": \"bob\", \"value\": 15.6}]}";;
  json_node *json = alloc_json_node();
  int err = json_decode(json_text, strlen(json_text), json);
  dict *v;
  json_dict_value(json, &v);
  array *e;
  get_ref(v, "statements", (void**) &e);
  dict *sube;
  array_get(e, 1, (void**) &sube);
  double amount;
  get_double(sube, "value", &amount);
  assert(15.6 == amount);
  char *se;
  get_ref(sube, "name", (void**) &se);
  assert(strcmp("bob", se) == 0);
  free_json_node(json);
}

void json_case_11(void) {
  char *json_text = "{ \"user\": {\"age\": 46, \"name\": \"jon\", \"balance\": 415.84, \"statements\": [{\"name\": \"joe\", \"value\": 215.84}, {\"name\": \"bob\", \"value\": 15.6}]}}";;
  json_node *json = alloc_json_node();
  int err = json_decode(json_text, strlen(json_text), json);
  json_node *v = alloc_json_node();
  json_node_by_key("user[age]", json, v);
  int iv;
  json_int_value(v, &iv);
  assert(46 == iv);
  json_node_by_key("[user][balance]", json, v);
  double dv;
  json_double_value(v, &dv);
  assert(415.84 == dv);
  json_node_by_key("[user][name]", json, v);
  char *sv;
  json_string_value(v, &sv);
  assert(strcmp("jon", sv) == 0);
  json_node_by_key("[user][statements]", json, v);
  array *e;
  json_array_value(v, &e);
  dict *sube;
  array_get(e, 1, (void**) &sube);
  double amount;
  get_double(sube, "value", &amount);
  assert(15.6 == amount);
  free_json_node(v);
  free_json_node(json);
}

#define CASES_LENGTH 11

int json_decode_tests(void) {
  case_test cases[CASES_LENGTH] = { &json_case_1, &json_case_2, &json_case_3,
                                    &json_case_4, &json_case_5, &json_case_6,
                                    &json_case_7, &json_case_8, &json_case_9,
                                    &json_case_10, &json_case_11 };
  run_tests(cases, CASES_LENGTH);

  return 0;
}