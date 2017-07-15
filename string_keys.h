#ifndef STRING_HASH_KEYS_H_
#define STRING_HASH_KEYS_H_

#include "hash.h"

int32_t string_hash_function (void *key);

hash_result string_hash_insert (struct const *kv src, struct *kv dest);

hash_result string_hash_remove (struct *kv);

int32_t string_hash_cmp (void const *k1, void const *k2);

#endif // STRING_HASH_KEYS_H_

