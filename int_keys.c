#include "int_keys.h"

int32_t int_hash_function (void *key) {
    return (int32_t)*key; 
}

hash_result int_hash_insert (struct const *kv src, struct *kv dest) {
    dest->k = malloc(sizeof(int));
    if (!dest->k)
        return HASH_FAIL_MALLOC;

    *dest->k = *(int *)src->k;
    dest->v = src->v;

    return HASH_SUCCESS;
}

hash_result int_hash_remove (struct *kv) {
    free(kv->k);
    kv->k = 0;

    return HASH_SUCCESS;
}

int32_t int_hash_cmp (void const *k1, void const *k2) {
    return (int32_t)(*k1) - (int32_t)(*k2);
}

