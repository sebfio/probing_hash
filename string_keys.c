#include "string_keys.h"

#include <string.h>

#define FNV_PRIME 16777619
#define FNV_OFFSET 0x811c9dc5

// FNV prime string hash algorithm
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
int32_t string_hash_function (void *key) {
    int32_t hash = FNV_OFFSET;

    char *curr = (char *)key;

    while (curr) {
        hash ^= curr;
        hash *= FNV_PRIME;
    }

    return hash;
}

hash_result string_hash_insert (struct const *kv src, struct *kv dest) {
    dest->k = malloc(strlen((char *)src->k));
    if (!dest->k)
        return HASH_FAIL_MALLOC;

    strcpy((char *)dest->k, (char *)src->k);
    dest->v = src->v;

    return HASH_SUCCESS;
}

hash_result string_hash_remove (struct *kv) {
    free(kv->k);
    kv->k = 0;

    return HASH_SUCCESS;
}

int32_t string_hash_cmp (void const *k1, void const *k2) {
    return string_hash_function(k1) - string_hash_function(k2);
}

