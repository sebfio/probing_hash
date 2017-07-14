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

struct hashmap {
    size_t table_size;
    size_t num_entries;
    void *table;
    // Hash function for every key in table
    hash_function   hash_fn;
    // How keys should be inserted
    hash_insert_key key_fn;
    // How a key should be removed
    hash_destroy    kv_remove;
}

// CRUD functions
struct hashmap *hashmap_init (hash_function hash_fn,
                              hash_insert_key hash_key_fn,
                              hash_destroy hash_kv_remove);

int32_t hashmap_destroy (struct hashmap *self);

int32_t hashmap_put (struct hashmap *self,
                     const struct *kv);

const void* hashmap_get (struct hashmap *self, const void *key);

int32_t hashmap_remove (struct hashmap *self, const void *key);

#endif // HASH_H_
