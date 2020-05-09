# Hashmap
 
An open-addressed, double-hashed hashmap implemented in C. I used gcc but the source code should complie alright in most C dialects.

## API
- `hm_hashmap *hm_init(void)`: Creates a hashmap.
- `void hm_destroy(hm_hashmap *hm)`: Destroys the hashmap and frees the allocated memory.
- `void hm_set(hm_hashmap *hm, const char *key, const char *val)`: inserts/resets a value in the hashmap.
- `char *hm_get(hm_hashmap *hm, const char *key)`: fetches the value associated with the given key. Returns `null` if the key does not exist.
- `void hm_delete(hm_hashmap *hm, const char *key)`: Deletes a key/value pair from the hashmap.

- `hm_iterator_t hm_iteratorInit(hm_hashmap_t hm)`: Creates an iterator over hashmap `hm`.
- `void hm_iteratorDestroy(hm_iterator_t iter)`: Destroys the iterator and frees memory.
- `bool hm_iteratorNext(hm_iterator_t iter)`: Moves the iterator's cursor to the next element in the hashmap. Returns false if the end of the hashmap is reached. Note that inserting new values into the hashmap may corrupt the iterator.
- `char *hm_iteratorKey(hm_iterator_t iter)`: Gets the key at the cursor in the iterator. Undefined behaviour if we reached the end of the hashmap.
- `void *hm_iteratorVal(hm_iterator_t iter)`: Gets the value at the cursor in the iterator. Undefined behaviour if we reached the end of the hashmap.

## Restrictions
- Keys can be strings only
- The maximum amounts of items that one hashmap can store is currently 33253889 (pretty arbitrary, I know). Attempts of inserting more items result in termination.

## Compile
`make`

## TODO
- Cross platform build (autotool, automake, libtool).
- Write tests.
- Optimise calculation of the second hash.
- Error handling.