#ifndef INT_HASH_KEYS_H_
#define INT_HASH_KEYS_H_

#include "hash.h"

int32_t int_hash_function (void *key);

hash_result int_hash_insert (struct const *kv src, struct *kv dest);

hash_result int_hash_remove (struct *kv);

int32_t int_hash_cmp (void const *k1, void const *k2);

#endif // INT_HASH_KEYS_H_

