#include "hash.h"

#include <stdlib.h>

#define RESIZE_LARGE_FACTOR 0.4
#define RESIZE_SMALL_FACTOR 0.05

typedef index int32_t;

static index hash_find_first_free_index (struct hashmap *self, void const *key);

struct hashmap *hashmap_init (hash_function hash_fn,
                              hash_insert_key hash_key_fn,
                              hash_destroy hash_kv_remove
                              hash_compare hash_kk_compare) {
    if (!hash_fn || !key_cmp || !hash_dest) {
        // Invalid support for hash table functions
        return NULL;
    }

    struct hashmap *hasht = malloc(sizeof(struct hashmap));

    if (!hasht)
        return NULL;

    hasht->kv_table = calloc(sizeof(kv), NEW_TABLE_SIZE);

    hasht->num_entries = 0;
    hasht->table_size = NEW_TABLE_SIZE;

    if (!hasht->table) {
        free(hasht)
        return NULL;
    }

    hasht->hash_fn          = hash_fn;
    hasht->insert_key_fn    = hash_key_fn;
    hasht->kv_remove        = hash_kv_remove;
    hasht->kk_compare       = hash_kk_compare;
    return hasht;
}

int32_t hashmap_destroy (struct hashmap *self) {
    if (!self)
        return -1;

    for (uint32_t i = 0; i < self->table_size; i++) {
        if (self->hasht->kv_table[i]) {
            // Remove shouldn't fail as it should only call free on code
            self->kv_remove(&self->hasht->kv_table[i]);
        }
    }
    
    free(self);

    return 0;
}

int32_t hashmap_put (struct hashmap *self,
                     struct const *kv) {
    if (!self || !kv)
        return -1;

    index i = hash_find_first_free_index(self, kv->k);

    const void *k_store = self->hash_insert_key(kv->k);

    if (!k_store) {
        // Out of memory, return failure occured due to part of function pointer code
        return -2;
    }
    
    self->kv_table[i].k = kv->k_store; 
    self->kv_table[i].v = kv->v;

    self->num_entries++;

    if (self->num_entries * 1.0 / self->table_size > RESIZE_LARGE_FACTOR) {
        // TODO: Resize table
    }
}

void * const hashmap_get (struct hashmap *self, void const *key) {
    if (!self || !key)
        return NULL;

    struct kv *kv = hash_kv_find(self, key);

    if (!kv)
        return NULL; 

    return kv->v;
}

int32_t hashmap_remove (struct hashmap *self, void const *key) {
    if (!self || !key) {
        return -1;
    }

    struct kv *kv = hash_kv_find(self, key);

    if (!kv) {
        return -1;
    }

    self->num_entries--;

    if (self->num_entries * 1.0 / self->table_size < RESIZE_SMALL_FACTOR) {
        // TODO: Resize table
    }
}

static index hash_find_first_free_index (struct hashmap *self, void const *key) {
    uint32_t const initial_index = self->hash_fn(key) % self->table_size;

    for (uint32_t i = 0; i < self->table_size; i++) {
        uint32_t key_location = (i + initial_index) / self->table_size;
        
        // Find the first free NULL key
        if (!self->table[key_location].k)
            return key_location;
    }
}

static struct kv *hash_kv_find (struct hashmap *self, void const *key) {
    int32_t key_hash = self->hash_fn(key);
    uint32_t initial_index = key_hash % self->table_size;

    for (uint32_t i = 0; i < self->table_size; i++) {
        uint32_t key_location = (i + initial_index) / self->table_size;

        if (!self->hash(self->table[key_location].k)) {
            // Key does not exist
            return NULL;
        }

        if (self->hash(self->table[key_location].k) != key_hash) {
            continue;
        }

        // Check exact keys match, if so return index
        if (self->kk_compare(self->table[key_location].k, key) == 0)
            return &self->table[key_location];
    }

    // Should never get here
    return NULL;
}
