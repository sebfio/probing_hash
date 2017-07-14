#include "hash.h"

#include <stdlib.h>


struct hashmap *hashmap_init (hash_function hash_fn,
                              hash_insert_key hash_key_fn,
                              hash_destroy hash_kv_remove) {
    if (!hash_fn || !key_cmp || !hash_dest) {
        // Invalid support for hash table functions
        return NULL;
    }

    struct hashmap *hasht = malloc(sizeof(struct hashmap));

    if (!hasht)
        return NULL;

    hasht->table_size = 0;
    hasht->num_entries = 0;
    hasht->kv_table = calloc(sizeof(kv), NEW_TABLE_SIZE);

    if (!hasht->table) {
        free(hasht)
        return NULL;
    }

    hasht->hash_fn      = hash_fn;
    hasht->key_fn       = hash_key_fn;
    hasht->kv_remove    = hash_kv_remove;

    return hasht;
}

int32_t hashmap_destroy (struct hashmap *self) {
    for (uint32_t i = 0; i < self->table_size; i++) {
        if (self->hasht->kv_table[i]) {
            self->kv_remove(&self->hasht->kv_table[i]);
        }
    }
    
    free(self);
}

int32_t hashmap_put (struct hashmap *self,
                     const struct *kv);

const void* hashmap_get (struct hashmap *self, const void *key);

int32_t hashmap_remove (struct hashmap *self, const void *key);
