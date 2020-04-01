#include<stdlib.h>
#include "dict.h"

#define PATH_ASCII_CONST 40

ERR insert(dict *root, char *key, void *value, bool is_ref);
ERR get(dict* root, char *key, void **value);
int path_index_for(char c);

dict* alloc_dict(){
    dict *root = malloc(sizeof(dict));
    for (int i = 0; i < PATHS_LENGTH; i++) {
        root->paths[i] = NULL;
    }
    root->should_free = false;
    root->value = NULL;
    return root;
}

ERR free_dict(dict* root) {
  if (root == NULL) {
    return 0;
  }

  for (int i = 0; i < PATHS_LENGTH; i++) {
    free_dict(root->paths[i]);
  }
  if(root->should_free) {
    free(root->value);
  }
  free(root);
  return 0;
}

int path_index_for(char c) {
  return c - PATH_ASCII_CONST;
}

ERR insert(dict* root, char *key, void *value, bool is_ref) {
  dict *curr = root;
  for(char *c = key; *c != '\0'; c++) {
    int path_index = path_index_for(*c);
    if(path_index < 0 || path_index >= PATHS_LENGTH) {
      return -1;
    }

    if(curr->paths[path_index] == NULL) {
      curr->paths[path_index] = alloc_dict();
    }
    curr = curr->paths[path_index];
  }
  curr->value = value;
  curr->should_free = !is_ref;
  return 0;
}

ERR insert_int(dict* root, char *key, int value) {
  int *int_ref = malloc(sizeof(int));
  *int_ref = value;
  return insert(root, key, int_ref, false);
}

ERR insert_double(dict* root, char *key, double value) {
  double *double_ref = malloc(sizeof(double));
  *double_ref = value;
  return insert(root, key, double_ref, false);
}

ERR insert_ref(dict* root, char *key, void *value) {
  return insert(root, key, value, true);
}

ERR get(dict* root, char *key, void **value) {
  dict *curr = root;
  for(char *c = key; *c != '\0'; c++) {
    int path_index = path_index_for(*c);
    if(curr->paths[path_index] == NULL) {
      return -1;
    }
    curr = curr->paths[path_index];
  }
  *value = curr->value;
  return 0;
}

ERR get_int(dict* root, char *key, int *value) {
  int *value_ref;
  get(root, key, (void **) &value_ref);
  *value = *value_ref;
  return 0;
}

ERR get_double(dict* root, char *key, double *value) {
  double *value_ref;
  get(root, key, (void **) &value_ref);
  *value = *value_ref;
  return 0;
}

ERR get_ref(dict* root, char *key, void **value) {
  return get(root, key, value);
}