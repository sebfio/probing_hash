#include "hash.h"

#include <stdlib.h>

#define RESIZE_LARGE_FACTOR 0.4
#define RESIZE_SMALL_FACTOR 0.05

#define abs(num)            \
    ((num) < 0) ? (num) * -1 : (num)

typedef int32_t index;

typedef enum resize_amt {
    RESIZE_QUARTER      = -4,
    RESIZE_HALF         = -2,
    RESIZE_DOUBLE       = 2,
    RESIZE_QUADRUPLE    = 4,
} e_resize_amt;

static index hash_find_first_free_index (struct hashmap *self, void const *key);
static struct kv *hash_kv_find (struct hashmap *self, void const *key);
static hash_result resize_table (struct hashmap *self, e_resize_amt resize_factor);

struct hashmap *hashmap_init (hash_function hash_fn,
                              hash_insert insert_fn,
                              hash_remove hash_kv_remove,
                              hash_compare hash_kk_compare) {
    if (!hash_fn || !insert_fn || !hash_kv_remove || !hash_kk_compare) {
        // Invalid support for hash table functions
        return NULL;
    }

    struct hashmap *hasht = malloc(sizeof(struct hashmap));

    if (!hasht)
        return NULL;

    hasht->kv_table = calloc(sizeof(struct kv), NEW_TABLE_SIZE);

    hasht->num_entries = 0;
    hasht->table_size = NEW_TABLE_SIZE;

    if (!hasht->kv_table) {
        free(hasht);
        return NULL;
    }

    hasht->hash_fn          = hash_fn;
    hasht->insert_fn        = insert_fn;
    hasht->kv_remove        = hash_kv_remove;
    hasht->kk_compare       = hash_kk_compare;
    return hasht;
}

hash_result hashmap_destroy (struct hashmap *self) {
    if (!self)
        return HASH_INVALID_PARAM;

    for (uint32_t i = 0; i < self->table_size; i++) {
        if (self->kv_table[i].k) {
            if (self->kv_remove(&self->kv_table[i])) {
                // Error occured
            }
        }
    }
    
    free(self);

    return HASH_SUCCESS; 
}

hash_result hashmap_put (struct hashmap *self,
                         struct kv const *kv) {
    if (!self || !kv)
        return HASH_INVALID_PARAM;

    index i = hash_find_first_free_index(self, kv->k);

    hash_result insert_result = self->insert_fn(kv, &self->kv_table[i]);
    
    if (insert_result)
        return insert_result;

    self->num_entries++;

    if (self->num_entries * 1.0 / self->table_size > RESIZE_LARGE_FACTOR) {
        return resize_table (self, RESIZE_QUADRUPLE);
    }

    return HASH_SUCCESS;
}

void * const hashmap_get (struct hashmap *self, void const *key) {
    if (!self || !key)
        return NULL;

    struct kv *kv = hash_kv_find(self, key);

    if (!kv)
        return NULL; 

    return kv->v;
}

hash_result hashmap_remove (struct hashmap *self, void const *key) {
    if (!self || !key) {
        return HASH_INVALID_PARAM;
    }

    struct kv *kv = hash_kv_find(self, key);

    hash_result removing_result = self->kv_remove(kv);

    // If error occured when removing then return that
    if (removing_result)
        return removing_result;

    self->num_entries--;

    if (self->num_entries * 1.0 / self->table_size < RESIZE_SMALL_FACTOR) {
        return resize_table (self, RESIZE_QUARTER);
    }
    
    return HASH_SUCCESS;
}

static index hash_find_first_free_index (struct hashmap *self, void const *key) {
    uint32_t const initial_index = self->hash_fn(key) % self->table_size;

    for (uint32_t i = 0; i < self->table_size; i++) {
        uint32_t key_location = (i + initial_index) / self->table_size;
        
        // Find the first free NULL key
        if (!self->kv_table[key_location].k)
            return key_location;
    }
}

static struct kv *hash_kv_find (struct hashmap *self, void const *key) {
    int32_t key_hash = self->hash_fn(key);
    uint32_t initial_index = key_hash % self->table_size;

    for (uint32_t i = 0; i < self->table_size; i++) {
        uint32_t key_location = (i + initial_index) / self->table_size;

        if (!self->hash_fn(self->kv_table[key_location].k)) {
            // Key does not exist
            return NULL;
        }

        if (self->hash_fn(self->kv_table[key_location].k) != key_hash) {
            continue;
        }

        // Check exact keys match, if so return index
        if (self->kk_compare(self->kv_table[key_location].k, key) == 0)
            return &self->kv_table[key_location];
    }

    // Should never get here
    return NULL;
}

static hash_result resize_table (struct hashmap *self, e_resize_amt resize_factor) {
    size_t original_table_size = self->table_size;
    if (resize_factor < 0) {
        // Table is being made smaller
        // Realloc should not fail. Go through table and rehash everything to
        // correct location
        
        for (uint32_t i = 0; i < original_table_size; i++) {
            if (!self->kv_table[i].k)
                continue;

            // TODO: Block here

            // Rehash key and insert it in the blocks at the correct size
            struct kv copy_kv = self->kv_table[i];

            // TODO: Make sure removed ok
            hashmap_remove(self, copy_kv.k);

            // Divide table by resize amt so that linear probing cannot place them in
            // regions which will be out of bounds once resized
            self->table_size /= abs(resize_factor);

            // TODO: Make sure inserted ok
            hashmap_put(self, &copy_kv);
            self->table_size = original_table_size;

            // Unblock here
        }
        
        // Nothing should be in the regions that will be removed
        size_t old_table_size = (sizeof(struct kv) * original_table_size); 

        self->table_size = old_table_size / abs(resize_factor);
        self->kv_table = realloc(self->kv_table, self->table_size);
        return HASH_SUCCESS;
    }
    else if (resize_factor > 0) {
        // Table is being made larger
        struct kv *new_table = realloc(self->kv_table, 
                sizeof(struct kv) * original_table_size * resize_factor);

        if (!new_table)
            return HASH_FAIL_MALLOC;

        for (uint32_t i = 0; i < original_table_size; i++) {
            // Loop through and move all old entries to new_table
            if (!self->kv_table[i].k)
                continue;

            // TODO: Block here
            struct kv copy_kv = self->kv_table[i];

            hashmap_remove(self, copy_kv.k);

            self->table_size *= resize_factor;

            hashmap_put(self, &copy_kv);
            self->table_size = original_table_size;

            // Unblock here
        }

        self->table_size = original_table_size * resize_factor;
        return HASH_SUCCESS;
    }
    else {
        return HASH_SUCCESS;
    }
}

