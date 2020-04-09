#ifndef LINHARES_DYNAMIC_ARRAY_H
#define LINHARES_DYNAMIC_ARRAY_H

#define AUTO_CAPACITY 0
#define DEFAULT_CAPACITY 20

typedef int ERR;
typedef struct _array {
  int length;
  int capacity;
  void **data;
} array;

#define ERR_OK 0
#define ERR_OUT_OF_BOUNDS -1

array* alloc_array(unsigned int capacity);
void array_push(array *arr, void *value);
void array_top(array *arr, /* output */ void **value);
void array_pop(array *arr);
ERR array_get(array *arr, unsigned int index, /* output */ void **value);
void array_set(array *arr, unsigned int index, void *value);
void free_array(array* arr);

#endif