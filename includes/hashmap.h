#pragma once

#include <stdbool.h>

typedef struct hm_hashmap_t *hm_hashmap_t;

hm_hashmap_t hm_init(void);
void hm_destroy(hm_hashmap_t hm);

void hm_set(hm_hashmap_t hm, const char *key, void *val);
void *hm_get(hm_hashmap_t hm, const char *key);
void hm_delete(hm_hashmap_t hm, const char *key);

typedef struct hm_iterator_t *hm_iterator_t;

hm_iterator_t hm_iteratorInit(hm_hashmap_t hm);
void hm_iteratorDestroy(hm_iterator_t iter);

bool hm_iteratorNext(hm_iterator_t iter);
char *hm_iteratorKey(hm_iterator_t iter);
void *hm_iteratorVal(hm_iterator_t iter);
