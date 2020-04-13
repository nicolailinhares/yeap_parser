#include "test_suite.h"

int main(void) {
  dict_tests();
  dynamic_array_tests();
  stack_tests();
  json_parse_tests();
  json_node_tests();
  json_decode_tests();
}