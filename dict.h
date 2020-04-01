#ifndef LINHARES_DICT_H
#define LINHARES_DICT_H

#include<stdbool.h>

#define PATHS_LENGTH 86

typedef int ERR;
typedef struct _trie {
  struct _trie *paths[PATHS_LENGTH];
  void *value;
  bool should_free;
} dict;

dict* alloc_dict(void);
ERR free_dict(dict *root);
ERR insert_int(dict *root, char *key, int value);
ERR insert_double(dict *root, char *key, double value);
ERR insert_ref(dict *root, char *key, void *value);
ERR get_int(dict *root, char *key, int *value);
ERR get_double(dict *root, char *key, double *value);
ERR get_ref(dict *root, char *key, void **value);

#endif