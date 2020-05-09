#include "cnt.h"

#include "hashmap.h"

hm_hashmap_t hm;

void cnt_setUp()
{
  hm = hm_init();
}

void cnt_tearDown()
{
  hm_destroy(hm);
}


void test_createHashmap()
{
  cnt_assertTrue(NULL != hm);
}

void test_retrieveSetValue()
{
  hm_set(hm, "key1", "value1");
  cnt_assertStringEqual("value1", (char *)hm_get(hm, "key1"));
}

void test_retrieveNonExistingValue()
{
  hm_set(hm, "key1", "value1");
  cnt_assertTrue(NULL == hm_get(hm, "key2"));
}

void test_retrieveDeletedValue()
{
  hm_set(hm, "key1", "value1");
  hm_delete(hm, "key1");
  cnt_assertTrue(NULL == hm_get(hm, "key1"));
}

void test_resize()
{
  #define N 2000
  char values[N][20];
  char keys[N][10];

  for (int i = 0; i < N; i++)
  {
    sprintf(keys[i], "key%d", i);
    sprintf(values[i], "value - %d", i);
    hm_set(hm, keys[i], (void *)values[i]);
  }

  for (int i = 0; i < N; i++)
  {
    cnt_assertStringEqual(values[i], (char *)hm_get(hm, keys[i]));
  }

  #undef N
}

void test_iterate()
{
  #define N 2000
  int visitedNodes[N] = {0};

  char values[N][20];
  char keys[N][10];

  for (int i = 0; i < N; i++)
  {
    sprintf(keys[i], "key%d", i);
    sprintf(values[i], "value - %d", i);
    hm_set(hm, keys[i], (void *)values[i]);
  }

  hm_iterator_t iter = hm_iteratorInit(hm);
  int i=0;
  while(hm_iteratorNext(iter))
  {
    int keyIndex;
    sscanf(hm_iteratorKey(iter), "key%d", &keyIndex);
    cnt_assertIntEqual(0, visitedNodes[keyIndex]);
    ++visitedNodes[keyIndex];
    ++i;
  }
  cnt_assertIntEqual(N, i);

  hm_iteratorDestroy(iter);
  #undef N
}


int main()
{
  cnt_start();

  cnt_run("Should create hashmap", test_createHashmap);
  cnt_run("Should read set value", test_retrieveSetValue);
  cnt_run("Should return void* for nonexisting value", test_retrieveNonExistingValue);
  cnt_run("Should return void* for deleted value", test_retrieveDeletedValue);
  cnt_run("Should preserve values after resize", test_resize);
  cnt_run("Should iterate through all values exactly once", test_iterate);

  cnt_end();
}