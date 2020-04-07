#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dict.h"
#include "json_node.h"

#define MAX_KEY_LENGTH 45

ERR json_dict_value(json_node *node, /* output */ dict **root);
ERR decode_key_subscription(char *key, /* output */ int *length, char **keys);
void alloc_key_to_position(char *key, int size, char **keys, int position);
ERR free_decoded_keys(int length, char **keys);

json_node* alloc_json_node() {
  json_node *node = malloc(sizeof(json_node));
  node->key = malloc(sizeof(char)*MAX_KEY_LENGTH);
  node->value = NULL;

  return node;
}

ERR json_dict_value(json_node *node, /* output */ dict **root) {
  *root = (dict *) node->value;
  return 0;
}

ERR json_int_value(json_node *node, /* output */ int *value) {
  *value = *((int*) node->value);
  return 0;
}

ERR json_double_value(json_node *node, /* output */ double *value) {
  *value = *((double*) node->value);
  return 0;
}

ERR json_node_value(json_node *node, /* output */ json_node *value) {
  return 0;
}

ERR json_array_value(json_node *node, /* output */ void *value[]) {
  return 0;
}

ERR json_string_value(json_node *node, /* output */ char **value) {
  *value = (char *) node->value;
  return 0;
}

ERR json_bool_value(json_node *node, /* output */ bool *value) {
  *value = *((bool*) node->value);
  return 0;
}

ERR json_node_by_key(char *key, json_node *node, /* output */ json_node *value) {
  char **keys = malloc(20*sizeof(char *));
  int length;
  decode_key_subscription(key, &length, keys);

  value->value = node->value;
  for(int i = 0; i < length; i++) {
    dict *dict_value = NULL;
    json_dict_value(value, &dict_value);
    int err = get_ref(dict_value, keys[i], &(value->value));
    if (err < 0) {
      return err;
    }
    strcpy(value->key, keys[i]);
  }

  free_decoded_keys(length, keys);
  return 0;
}

ERR json_decode(char *json_text, int length, /* output */ json_node *node) {
  return 0;
}

ERR free_json_node(json_node *node) {
  free(node->key);
  free(node);

  return 0;
}

ERR decode_key_subscription(char *key, /* output */ int *length, char **keys) {
  int idx = 0;
  int key_idx = 0;
  char curr_key[MAX_KEY_LENGTH];
  for(int i = 0, n = strlen(key); i < n; i++) {
    if (key[i] == '[' && key_idx == 0){
      continue;
    } else if(key[i] == ']' || key[i] == '[') {
      curr_key[key_idx++] = '\0';
      alloc_key_to_position(curr_key, key_idx + 1, keys, idx);
      key_idx = 0;
      idx++;
    } else {
      curr_key[key_idx++] = key[i];
    }
  }

  if(idx == 0 && key_idx > 0) {
    curr_key[key_idx++] = '\0';
    alloc_key_to_position(curr_key, key_idx + 1, keys, idx);
    idx++;
  }

  *length = idx;

  return 0;
}

void alloc_key_to_position(char *key, int size, char **keys, int position) {
  char *found_key = malloc(sizeof(char)*size);
  strcpy(found_key, key);
  keys[position] = found_key;
}

ERR free_decoded_keys(int length, char **keys) {
  for(int i = 0; i < length; i++) {
    free(keys[i]);
  }

  return 0;
}