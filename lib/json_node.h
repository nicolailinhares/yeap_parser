#ifndef YETAPARSER_JSON_NODE_H
#define YETAPARSER_JSON_NODE_H
#include <stdbool.h>
#include "dynamic_array.h"
#include "dict.h"

typedef int ERR;
typedef struct _json_node {
  char *key;
  void *value;
} json_node;

json_node* alloc_json_node(void);
ERR json_int_value(json_node *node, /* output */ int *value);
ERR json_double_value(json_node *node, /* output */ double *value);
ERR json_dict_value(json_node *node, /* output */ dict **value);
ERR json_array_value(json_node *node, /* output */ array **value);
ERR json_string_value(json_node *node, /* output */ char **value);
ERR json_bool_value(json_node *node, /* output */ bool *value);
ERR json_node_by_key(char *key, json_node *node, /* output */ json_node *value);
ERR free_json_node(json_node *node);

#endif