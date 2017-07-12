#include "hash.h"

#include <stdlib.h>


struct hashmap *hashmap_init (hash_function hash_fn,
							  key_compare key_cmp,
							  hash_dest) {
	if (!hash_fn || !key_cmp || !hash_dest) {
		// Invalid support for hash table functions
		return NULL;
	}

	struct hashmap *hasht = malloc(sizeof(struct hashmap));

	if (!hasht)
		return NULL;

	hasht->table_size = 0;
	hasht->num_entries = 0;
	hasht->kv_table = malloc(sizeof(void *) * NEW_TABLE_SIZE);

	if (!hasht->table) {
		free(hasht)
		return NULL;
	}

	hasht->hash_fn 		= hash_fn;
	hasht->key_cmp 		= key_cmp;
	hasht->hash_dest 	= hash_dest;

	return hasht;
}

int32_t hashmap_destroy (struct hashmap *self);

int32_t hashmap_put (struct hashmap *self,
					 const struct *kv);

const void* hashmap_get (struct hashmap *self, const void *key);

int32_t hashmap_remove (struct hashmap *self, const void *key);
