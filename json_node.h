#ifndef LINHARES_JSON_NODE_H
#define LINHARES_JSON_NODE_H

typedef int ERR;
typedef struct _json_node {
  char *key;
  void *value;
} json_node;

json_node* alloc_json_node(void);
ERR json_int_value(json_node *node, /* output */ int *value);
ERR json_double_value(json_node *node, /* output */ double *value);
ERR json_node_value(json_node *node, /* output */ json_node *value);
ERR json_array_value(json_node *node, /* output */ void *value[]);
ERR json_string_value(json_node *node, /* output */ char **value);
ERR json_bool_value(json_node *node, /* output */ bool *value);
ERR json_node_by_key(char *key, json_node *node, /* output */ json_node *value);
ERR json_decode(char *json_text, int length, /* output */ json_node *node);
ERR free_json_node(json_node *node);

#endif