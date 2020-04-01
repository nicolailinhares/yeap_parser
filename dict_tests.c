#include <stdio.h>
#include <string.h>

#include "dict.h"
#include "unit_test.h"

void case_1(void) {
  //testing integers
  char *keys[10] = {"joao", "pedro", "tristan", "age", "count", "id", "publicId", "public_id", "age2", "number"};
  int values[10] = {-1, 0, 1, 1000, 200000, -999999, 28, 423984, 90123, -94230492};
  dict *root = alloc_dict();
  for(int i = 0; i < 10; i++) {
    insert_int(root, keys[i], values[i]);
  }

  for(int i = 9; i >= 0; i--) {
    int v;
    get_int(root, keys[i], &v);
    assert(v == values[i]);
  }

  free_dict(root);
}

void case_2(void) {
  //testing doubles
  char *keys[10] = {"joao", "pedro", "tristan", "age", "count", "id", "publicId", "public_id", "age2", "number"};
  double values[10] = {-12.8, 0, 18.5, 1000, 200000.35, -999999.67, 28.45345, 423984.55, 90123.22, -94230492.32};
  dict *root = alloc_dict();
  for(int i = 0; i < 10; i++) {
    insert_double(root, keys[i], values[i]);
  }

  for(int i = 9; i >= 0; i--) {
    double v;
    get_double(root, keys[i], &v);
    assert(v == values[i]);
  }

  free_dict(root);
}

void case_3(void) {
  //testing references
  char *keys[10] = {"01234", "zzZZz", "tris-tan", "age_", "}}co}}", "id", "publicId", "public_id", "age2", "number"};
  char *values[10] = {"bombril", "sabao", "pequeno", "curto", "rapido", "legal", "top", "muito top", "curto", "um pouco mais longo para testar um pouco melhor a alocação de memória e se as coisas continuam em sequencia"};
  dict *root = alloc_dict();
  for(int i = 0; i < 10; i++) {
    insert_ref(root, keys[i], values[i]);
  }

  for(int i = 9; i >= 0; i--) {
    char *v = NULL;
    get_ref(root, keys[i], (void**) &v);
    assert(strcmp(v,values[i]) == 0);
  }

  free_dict(root);
}

void case_4(void) {
  //testing overwriting a key
  int v1 = 55;
  int v2 = -99978;
  dict *root = alloc_dict();
  insert_ref(root, "age{[2]}", &v1);
  insert_ref(root, "{[2]}age", &v1);
  insert_ref(root, "age{[2]}", &v2);
  int *v = NULL;
  get_ref(root, "age{[2]}", (void**) &v);
  assert(*v == v2);
  get_ref(root, "{[2]}age", (void**) &v);
  assert(*v == v1);
}

#define CASES_LENGTH 4

int dict_tests(void) {
  case_test cases[CASES_LENGTH] = {&case_1, &case_2, &case_3, &case_4};
  run_tests(cases, CASES_LENGTH);

  return 0;
}
