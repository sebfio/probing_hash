#ifndef HASH_H_
#define HASH_H_

#include <stdint.h>
#include <stddef.h>

#define NEW_TABLE_SIZE 97

// TODO: Use these as return values
typedef enum {
    HASH_FAIL_OTHER     = -4,
    HASH_INVALID_PARAM  = -3,
    HASH_FAIL_PFUNCTION = -2,
    HASH_FAIL_MALLOC    = -1,
    HASH_SUCCESS        = 0,
} hash_result;

struct kv {
    void *k;
    void *v;
};

typedef int32_t (*hash_function)(void const *key);
typedef hash_result (*hash_insert)(struct kv const *src, struct kv *dest);
typedef hash_result (*hash_remove)(struct kv*);
typedef int32_t (*hash_compare)(void const *kv1, void const *kv2);

struct hashmap {
    size_t table_size;
    size_t num_entries;
    struct kv *kv_table;
    // Hash function for every key in table
    hash_function hash_fn;
    // How kv should be inserted
    hash_insert insert_fn;
    // How a key should be removed
    hash_remove kv_remove;
    // Expensive compare to make sure keys do match before returning
    // hit
    hash_compare kk_compare;
};

// CRUD functions
struct hashmap *hashmap_init (hash_function hash_fn,
                              hash_insert insert_fn,
                              hash_remove kv_remove,
                              hash_compare kk_compare);

int32_t hashmap_destroy (struct hashmap *self);

int32_t hashmap_put (struct hashmap *self,
                     struct kv const *);

void * const hashmap_get (struct hashmap *self, void const *key);

int32_t hashmap_remove (struct hashmap *self, void const *key);

#endif // HASH_H_
