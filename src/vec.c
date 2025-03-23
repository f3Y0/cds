#include "cds/def.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <cds/vec.h>

#define ASSERT(Expr, ErrFmt, ...) _CDS_ASSERT(Expr, "cds_vec", ErrFmt, ##__VA_ARGS__)

#define ASSERT_NOT_NULL(Self) ASSERT((Self), "'self' may not be NULL");

static cds_err_t resize(
    cds_vec_t *const self,
    const size_t new_capacity
) {
    if(new_capacity == self->capacity) return CDS_ERR_OK;

    if(new_capacity == 0) {
        free(self->buffer);
        self->buffer = NULL;
    } else {
        uint8_t *const new_buffer = realloc(self->buffer, new_capacity * self->type->size);
        if(!new_buffer) return CDS_ERR_OOM;

        self->buffer = new_buffer;
        self->capacity = new_capacity;
    }

    return CDS_ERR_OK;
}

static size_t next_capacity(
    size_t current_capacity,
    const size_t min_next_capacity
) {
    if(min_next_capacity < CDS_VEC_GROWTH_MULTIPLIER_ACTIVATION_THRESHOLD) {
        return CDS_VEC_GROWTH_MULTIPLIER_ACTIVATION_THRESHOLD;
    } else if(current_capacity < CDS_VEC_GROWTH_MULTIPLIER_ACTIVATION_THRESHOLD) {
        current_capacity = CDS_VEC_GROWTH_MULTIPLIER_ACTIVATION_THRESHOLD;
    }

    size_t new_capacity = current_capacity;
    do {
        new_capacity *= CDS_VEC_GROWTH_MULTIPLIER;
    } while(new_capacity < min_next_capacity);

    return new_capacity;
}

static cds_err_t grow_if_needed(
    cds_vec_t *const self,
    const size_t additional_space_needed
) {
    if(self->size + additional_space_needed > self->capacity) {
        return resize(self, next_capacity(self->capacity, self->size + additional_space_needed));
    }

    return CDS_ERR_OK;
}

#define GROW_IF_NEEDED(Self, AdditionalCapacity) \
do { \
    const cds_err_t err = grow_if_needed((Self), (AdditionalCapacity)); \
    if(err != CDS_ERR_OK) { \
        ASSERT(0, "Failed to allocate an additional %zu capacity (error code: %d)", (AdditionalCapacity), (int)err); \
    } \
} while(0)

cds_err_t cds_vec_construct(
    cds_vec_t *const self,
    const cds_type_t *const type
) {
    return cds_vec_construct_with_capacity(self, type, 0);
}

cds_err_t cds_vec_construct_with_capacity(
    cds_vec_t *const self,
    const cds_type_t *const type,
    const size_t capacity 
) {
    *self = (cds_vec_t){ 0 };
    self->type = type;

    return resize(self, capacity);
}

void cds_vec_destruct(
    cds_vec_t *const self
) {
    for(size_t i = 0; i < self->size; i++) {
        self->type->destruct(cds_vec_get_at(self, i));
    }
    free(self->buffer);

    *self = (cds_vec_t){ 0 };
}

cds_err_t cds_vec_add(
    cds_vec_t *const self,
    void *const object
) {
    GROW_IF_NEEDED(self, (size_t)1);

    self->size++;
    self->type->move(
        cds_vec_get_back(self),
        object
    );

    return CDS_ERR_OK;
}

cds_err_t cds_vec_add_at(
    cds_vec_t *const self,
    void *const object,
    const size_t index
) {
    GROW_IF_NEEDED(self, (size_t)1);

    for(size_t i = self->size; i > index; i--) {
        self->type->move(
            _cds_vec_get_at_unchecked(self, i),
            _cds_vec_get_at_unchecked(self, i-1)
        );
    }

    self->type->move(
        cds_vec_get_at(self, index),
        object
    );
    self->size++;

    return CDS_ERR_OK;
}

bool cds_vec_remove(
    cds_vec_t *const self,
    const void *const object
) {
    for(size_t i = 0; i < self->size; i++) {
        if(self->type->cmp(cds_vec_get_at(self, i), object) == 0) {
            cds_vec_remove_at(self, i);
            return true;
        }
    }

    return false;
}

bool cds_vec_remove_fast(
    cds_vec_t *const self,
    const void *const object
) {
    for(size_t i = 0; i < self->size; i++) {
        if(self->type->cmp(cds_vec_get_at(self, i), object) == 0) {
            cds_vec_remove_at_fast(self, i);
            return true;
        }
    }

    return false;
}

void cds_vec_remove_at(
    cds_vec_t *const self,
    const size_t index
) {
    self->type->destruct(cds_vec_get_at(self, index));
    self->size--;
    for(size_t i = index; i < self->size; i++) {
        self->type->move(
            cds_vec_get_at(self, i),
            cds_vec_get_at(self, i+1)
        );
    }
}

void cds_vec_remove_at_fast(
    cds_vec_t *const self,
    const size_t index
) {
    self->type->destruct(cds_vec_get_at(self, index));
    self->size--;
    if(!cds_vec_is_empty(self)) {
        self->type->move(
            cds_vec_get_at(self, index),
            _cds_vec_get_at_unchecked(self, self->size)
        );
    }
}

void *cds_vec_get_at(
    const cds_vec_t *const self,
    const size_t index
) {
    return _cds_vec_get_at_unchecked(self, index);
}

void *cds_vec_get_front(
    const cds_vec_t *const self
) {
    return cds_vec_get_at(self, 0);
}

void *cds_vec_get_back(
    const cds_vec_t *const self
) {
    return cds_vec_get_at(self, self->size - 1);
}

bool cds_vec_is_empty(
    const cds_vec_t *const self
) {
    return self->size > 0;
}

size_t cds_vec_get_size(
    const cds_vec_t *const self
) {
    return self->size;
}

void *cds_vec_get_raw(
    const cds_vec_t *const self
) {
    return self->buffer;
}

void *_cds_vec_get_at_unchecked(
    const cds_vec_t *const self,
    const size_t index
) {
    return self->buffer + (index * self->type->size);
}

