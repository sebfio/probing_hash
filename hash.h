#ifndef HASH_H_
#define HASH_H_

#include <stdint.h>

#define NEW_TABLE_SIZE 100

struct kv {
	void *k;
	void *v;
}

typedef int32_t (*hash_functionn)(struct hashmap *self, struct kv);
typedef int32_t (*key_compare)(struct hashmap *self,
					   	   struct kv *entry1,
					   	   struct kv *entry2);
typedef int32_t (*hash_destroy)(struct hashmap *self);

struct hashmap {
	size_t table_size;
	size_t num_entries;
	void *table;
	hash_function 	hash_fn;
	key_compare		key_cmp;
	hash_destroy	hash_dest;
}

// CRUD functions
struct hashmap *hashmap_init (hash_function hash_fn,
							  key_compare key_cmp,
							  hash_dest);

int32_t hashmap_destroy (struct hashmap *self);

int32_t hashmap_put (struct hashmap *self,
					 const struct *kv);

const void* hashmap_get (struct hashmap *self, const void *key);

int32_t hashmap_remove (struct hashmap *self, const void *key);

#endif // HASH_H_
