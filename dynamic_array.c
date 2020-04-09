#include <stdlib.h>
#include "dynamic_array.h"

#define ARRAY_IDX arr->length - 1

void realloc_array(array *arr);

array* alloc_array(unsigned int capacity) {
  array* arr = malloc(sizeof(array));
  if(capacity == 0) {
    arr->capacity = DEFAULT_CAPACITY;
  } else {
    arr->capacity = capacity;
  }
  arr->length = 0;
  arr->data = malloc(sizeof(void*)*arr->capacity);

  return arr;
}

void array_push(array *arr, void *value) {
  arr->length++;
  if(arr->length > arr->capacity) {
    arr->capacity *= 2;
    arr->data = realloc(arr->data, sizeof(void*)*arr->capacity);
  }

  arr->data[ARRAY_IDX] = value;
}

void array_top(array *arr, /* output */ void **value) {
  *value = arr->data[ARRAY_IDX];
}

void array_pop(array *arr) {
  arr->length--;
  if(arr->length < arr->capacity/2) {
    arr->capacity /= 2;
    arr->data = realloc(arr->data, sizeof(void*)*arr->capacity);
  }
}

ERR array_get(array *arr, unsigned int index, /* output */ void **value) {
  int err = ERR_OK;
  if(index >= 0 && index < arr->length) {
    *value = arr->data[index];
  } else {
    *value = NULL;
    err = ERR_OUT_OF_BOUNDS;
  }

  return err;
}

void array_set(array *arr, unsigned int index, void *value) {
  unsigned int target_size = index + 1;
  if(arr->capacity < target_size) {
    arr->capacity = target_size;
    realloc_array(arr);
  }

  arr->data[index] = value;
  arr->length = target_size;
}

void realloc_array(array *arr) {
  arr->data = realloc(arr->data, sizeof(void*)*arr->capacity);
}

void free_array(array* arr) {
  arr->capacity = 0;
  arr->length = 0;
  free(arr->data);
  free(arr);
}