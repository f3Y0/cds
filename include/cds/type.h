#pragma once

#include <stddef.h>

#include <cds/def.h>

typedef struct cds_type {
    size_t size;

    cds_copy_f copy;
    cds_move_f move;
    cds_destruct_f destruct;
    cds_cmp_f cmp;
} cds_type_t;

#define CDS_TYPE(Size, CopyFn, MoveFn, DestructFn, CmpFn) (cds_type_t) \
{ \
    .size = (Size), \
    .copy = (cds_copy_f)(CopyFn), \
    .move = (cds_move_f)(MoveFn), \
    .destruct = (cds_destruct_f)(DestructFn), \
    .cmp = (cds_cmp_f)(CmpFn) \
}

static inline void _cds_type_empty_destruct(void *const self) {
    (void)self;
}

#define CDS_TYPE_EMPTY_DESTRUCTOR _cds_type_empty_destruct

static inline int _cds_type_int_copy(int *const self, const int *const other) {
    *self = *other;
    return 0;
}

static inline void _cds_type_int_move(int *const self, int *const other) {
    *self = *other;
}

static inline int _cds_type_int_cmp(const int *const self, const int *const other) {
    return *self - *other;
}

static const cds_type_t CDS_TYPE_INT = CDS_TYPE(
    sizeof(int), 
    _cds_type_int_copy, 
    _cds_type_int_move, 
    CDS_TYPE_EMPTY_DESTRUCTOR, 
    _cds_type_int_cmp
);

