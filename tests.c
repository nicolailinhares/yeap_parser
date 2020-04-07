#include "test_suite.h"

int main(void) {
  dict_tests();
  stack_tests();
  json_decode_tests();
  json_node_tests();
}