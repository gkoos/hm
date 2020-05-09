#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "hashmap.h"
#include "hashfuncs.h"

#define INITIAL_BASE_SIZE 100

#define PRIME1 151
#define PRIME2 163

#define HIGH_DENSITY 70

typedef struct
{
  char *key;
  void *value;
} hm_item;

struct hm_hashmap_t
{
  long size;
  long count;
  hm_item **items;
};

struct hm_iterator_t
{
  hm_hashmap_t hm;
  long cursor;
  bool finished;
  char *key;
  void *val;
};

static hm_item HM_DELETED_ITEM = {NULL, NULL};

enum itemType
{
  empty,
  deleted,
  normal
};

static long sizes[] = {1009, 2027, 8111, 16223, 32467, 64937, 129887, 259781, 519577, 1039169, 2078339, 4156709, 8313433, 16626941, 33253889};

static int currentSizeIndex = 0;

#define sizeLength sizeof(sizes) / sizeof(sizes[0])

typedef struct
{
  long index;
  enum itemType type;
} itemIndex;

/*
 * private methods
*/

static hm_item *hm_newItem(const char *k, void *v)
{
  hm_item *item = malloc(sizeof(hm_item));
  item->key = strdup(k);
  item->value = v;
  return item;
}

static hm_hashmap_t hm_initSized(const int newSize)
{
  hm_hashmap_t hm = malloc(sizeof(*hm));
  hm->size = newSize;
  hm->count = 0;
  hm->items = calloc((size_t)hm->size, sizeof(hm_item *));

  return hm;
}

static void hm_resize(hm_hashmap_t hm, const long newSize)
{
  if (newSize >= sizes[sizeLength - 1])
  {
    return;
  }

  hm_hashmap_t tmpHm = hm_initSized(newSize);
  if (tmpHm == NULL)
  {
    return; // if there wasn't enough memory, just return
  }

  for (int i = 0; i < hm->size; i++)
  {
    hm_item *item = hm->items[i];
    if (item != NULL && item != &HM_DELETED_ITEM)
    {
      hm_set(tmpHm, item->key, item->value);
    }
  }

  hm->count = tmpHm->count;

  const long tmpSize = hm->size;
  hm->size = tmpHm->size;
  tmpHm->size = tmpSize;

  hm_item **tmpItems = hm->items;
  hm->items = tmpHm->items;
  tmpHm->items = tmpItems;

  hm_destroy(tmpHm);
}

static itemIndex hm_findItemIndex(hm_hashmap_t hm, const char *key)
{
  long index = strHash(key, PRIME1, hm->size);

  hm_item *item = hm->items[index];

  int i = 1;
  while (item != NULL)
  {
    if (item == &HM_DELETED_ITEM)
    {
      return (itemIndex){index, deleted};
    }
    else if (strcmp(item->key, key) == 0)
    {
      return (itemIndex){index, normal};
    }

    index = strDoubleHash(key, hm->size, PRIME1, PRIME2, i);
    item = hm->items[index];
    i++;

    if (i > hm->size)
    {
      // out of memory
      exit(1);
    }
  }
  return (itemIndex){index, empty};
}

static void hm_deleteItem(hm_item *item)
{
  free(item->key);
  free(item);
}

static void hm_iteratorFindNext(hm_iterator_t it)
{
  long cur = it->cursor + 1;
  while (cur < it->hm->size)
  {
    if (it->hm->items[cur] != NULL && it->hm->items[cur] != &HM_DELETED_ITEM)
    {
      it->cursor = cur;
      it->key = it->hm->items[cur]->key;
      it->val = it->hm->items[cur]->value;
      return;
    }

    ++cur;
  }

  it->finished = true;
}

/*
 * public methods
*/

hm_hashmap_t hm_init()
{
  return hm_initSized(sizes[0]);
}

void hm_destroy(hm_hashmap_t hm)
{
  for (long i = 0; i < hm->size; i++)
  {
    hm_item *item = hm->items[i];
    if (item != NULL && item != &HM_DELETED_ITEM)
    {
      hm_deleteItem(item);
    }
  }
  free(hm->items);
  free(hm);
}

void hm_set(hm_hashmap_t hm, const char *key, void *value)
{
  itemIndex i = hm_findItemIndex(hm, key);

  if (i.type == normal)
  {
    free(hm->items[i.index]->value);
    hm->items[i.index]->value = value;

    return;
  }

  hm_item *item = hm_newItem(key, value);
  hm->items[i.index] = item;
  hm->count++;

  long long load = hm->count * 100 / hm->size;
  if (load > HIGH_DENSITY && currentSizeIndex < (int)(sizeLength - 1))
  {
    hm_resize(hm, sizes[++currentSizeIndex]);
  }
}

void *hm_get(hm_hashmap_t hm, const char *key)
{
  itemIndex i = hm_findItemIndex(hm, key);
  return i.type == empty || i.type == deleted ? NULL : hm->items[i.index]->value;
}

void hm_delete(hm_hashmap_t hm, const char *key)
{
  itemIndex i = hm_findItemIndex(hm, key);

  if (i.type == empty || i.type == deleted)
  {
    return;
  }

  hm_deleteItem(hm->items[i.index]);
  hm->items[i.index] = &HM_DELETED_ITEM;
  hm->count--;
}

hm_iterator_t hm_iteratorInit(hm_hashmap_t hm)
{
  hm_iterator_t it = malloc(sizeof(*it));
  it->hm = hm;
  it->cursor = -1;
  it->finished = false;

  return it;
}

void hm_iteratorDestroy(hm_iterator_t iter)
{
  free(iter);
}

bool hm_iteratorNext(hm_iterator_t iter)
{
  if (iter->finished)
    return false;

  hm_iteratorFindNext(iter);
  return !iter->finished;
}

char *hm_iteratorKey(hm_iterator_t iter)
{
  return iter->hm->items[iter->cursor]->key;
}

void *hm_iteratorVal(hm_iterator_t iter)
{
  return iter->hm->items[iter->cursor]->value;
}
