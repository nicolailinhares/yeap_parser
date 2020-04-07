#include<string.h>
#include<stdio.h>
#include "dict.h"
#include "json_node.h"
#include "unit_test.h"

void json_node_case_1(void) {
  //"{\"age\": 46}";
  dict *root = alloc_dict();
  insert_int(root, "age", 46);
  json_node *node = alloc_json_node();
  node->value = (void *) root;
  int value;
  json_node *out_node = alloc_json_node();
  json_node_by_key("[age]", node, out_node);
  json_int_value(out_node, &value);
  assert(46 == value);
  free_dict(root);
  free_json_node(node);
}

void json_node_case_2(void) {
  //"12";
  int ref_value = 12;
  int value;
  json_node *node = alloc_json_node();
  node->value = (void *) &ref_value;
  json_int_value(node, &value);
  assert(ref_value == value);
  free_json_node(node);
}

void json_node_case_3(void) {
  //"47.89";
  double ref_value = 12;
  double value;
  json_node *node = alloc_json_node();
  node->value = (void *) &ref_value;
  json_double_value(node, &value);
  assert(ref_value == value);
  free_json_node(node);
}

void json_node_case_4(void) {
  //"\"jon\"";
  char *ref_value = "jon";
  char *value;
  json_node *node = alloc_json_node();
  node->value = (void *) ref_value;
  json_string_value(node, &value);
  assert(strcmp(ref_value, value) == 0);
  free_json_node(node);
}

void json_node_case_5(void) {
  //"[1, 1.8, \"jon\", [1 , 2, 3], {\"age\": 46, \"name\": \"jon\", \"balance\": 415.84}]";
  printf(" json_node_case_5:skipped ");
  //assert(0 == err);
}

void json_node_case_6(void) {
  //"null"
  json_node *value = NULL;
  json_node *node = alloc_json_node();
  json_node_value(node, value);
  assert(NULL == value);
  free_json_node(node);
}

void json_node_case_7(void) {
  //"true";
  bool ref_value = true;
  bool value;
  json_node *node = alloc_json_node();
  node->value = (void *) &ref_value;
  json_bool_value(node, &value);
  assert(ref_value == value);
  free_json_node(node);
}

void json_node_case_8(void) {
  //"{\"user\": {\"name\": \"jon\", \"age\": 46}}";
  dict *root = alloc_dict();
  dict *user_root = alloc_dict();
  insert_ref(user_root, "name", "jon");
  insert_int(user_root, "age", 46);
  insert_ref(root, "user", (void *) user_root);

  json_node *node = alloc_json_node();
  node->value = (void *) root;

  int age;
  json_node *out_node = alloc_json_node();
  json_node_by_key("user[age]", node, out_node);
  json_int_value(out_node, &age);
  assert(46 == age);

  char *name;
  json_node_by_key("user[name]", node, out_node);
  json_string_value(out_node, &name);
  assert(strcmp("jon", name) == 0);

  free_dict(user_root);
  free_dict(root);
  free_json_node(node);
}

#define CASES_LENGTH 8

int json_node_tests(void) {
  case_test cases[CASES_LENGTH] = { &json_node_case_1, &json_node_case_2, &json_node_case_3,
                                    &json_node_case_4, &json_node_case_5, &json_node_case_6,
                                    &json_node_case_7, &json_node_case_8 };
  run_tests(cases, CASES_LENGTH);

  return 0;
}