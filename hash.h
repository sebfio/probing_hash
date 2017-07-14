#ifndef HASH_H_
#define HASH_H_

#include <stdint.h>

#define NEW_TABLE_SIZE 97

struct kv {
    void *k;
    void *v;
}

typedef int32_t (*hash_function)(void *key);
typedef int32_t (*hash_insert_key)(struct *kv);
typedef int32_t (*hash_destroy)(struct *kv);
typedef int32_t (*hash_compare)(struct const *kv1, struct const *kv2);

struct hashmap {
    size_t table_size;
    size_t num_entries;
    void *table;
    // Hash function for every key in table
    hash_function   hash_fn;
    // How keys should be inserted
    hash_insert_key insert_key_fn;
    // How a key should be removed
    hash_destroy    kv_remove;
    // Expensive compare to make sure keys do match before returning
    // hit
    hash_compare    kk_compare;
}

// CRUD functions
struct hashmap *hashmap_init (hash_function hash_fn,
                              hash_insert_key hash_key_fn,
                              hash_destroy hash_kv_remove,
                              hash_compare kk_compare);

int32_t hashmap_destroy (struct hashmap *self);

int32_t hashmap_put (struct hashmap *self,
                     struct const *kv);

void * const hashmap_get (struct hashmap *self, void const *key);

int32_t hashmap_remove (struct hashmap *self, void const *key);

#endif // HASH_H_
