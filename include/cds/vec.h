#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <cds/type.h>

#ifndef CDS_VEC_GROWTH_MULTIPLIER 
    #define CDS_VEC_GROWTH_MULTIPLIER 1.5f
#endif

#ifndef CDS_VEC_GROWTH_MULTIPLIER_ACTIVATION_THRESHOLD
    #define CDS_VEC_GROWTH_MULTIPLIER_ACTIVATION_THRESHOLD 10
#endif

typedef struct cds_vec {
    const cds_type_t *type;

    size_t size;
    size_t capacity;
    uint8_t *buffer;
} cds_vec_t;

cds_err_t cds_vec_construct(
    cds_vec_t *self,
    const cds_type_t *type
);

cds_err_t cds_vec_construct_with_capacity(
    cds_vec_t *self,
    const cds_type_t *type,
    size_t capacity 
);

void cds_vec_destruct(
    cds_vec_t *self
);

cds_err_t cds_vec_add(
    cds_vec_t *self,
    void *object
);

cds_err_t cds_vec_add_at(
    cds_vec_t *self,
    void *object,
    size_t index
);

bool cds_vec_remove(
    cds_vec_t *self,
    const void *object
);

bool cds_vec_remove_fast(
    cds_vec_t *self,
    const void *object
);

void cds_vec_remove_at(
    cds_vec_t *self,
    size_t index
);

void cds_vec_remove_at_fast(
    cds_vec_t *self,
    size_t index
);

void *cds_vec_get_at(
    const cds_vec_t *self,
    size_t index
);

void *cds_vec_get_front(
    const cds_vec_t *self
);

void *cds_vec_get_back(
    const cds_vec_t *self
);

bool cds_vec_is_empty(
    const cds_vec_t *self
);

size_t cds_vec_get_size(
    const cds_vec_t *self
);

void *cds_vec_get_raw(
    const cds_vec_t *self
);

void *_cds_vec_get_at_unchecked(
    const cds_vec_t *self,
    size_t index
);

#define CDS_VEC_FOREACH(Self, CurrentElementVarName, CurrentElementVarType) \
for( \
    CurrentElementVarType CurrentElementVarName = cds_vec_get_front((Self)); \
    (uintptr_t)CurrentElementVarName <= (uintptr_t)cds_vec_get_back((Self)); \
    CurrentElementVarName = (CurrentElementVarType)((uintptr_t)CurrentElementVarName + (Self)->type->size) \
)

#define CDS_VEC_FORALL(Self, Fn, ...) \
do { \
    CDS_VEC_FOREACH(Self, elm, void *) { \
        (Fn)(elm, ##__VA_ARGS__); \
    } \
} while(0)

#define CDS_VEC_GET_AT(Self, Index, Type) (Type *)cds_vec_get_at((Self), (Index))

#define CDS_VEC_GET_AT_DEREF(Self, Index, Type) *(Type *)cds_vec_get_at((Self), (Index))

