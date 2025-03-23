#pragma once

#include <stdio.h>

typedef enum {
    CDS_ERR_OK=0,
    CDS_ERR_OOM,
} cds_err_t;

typedef int (*cds_copy_f)(void *self, const void *other);

typedef void (*cds_move_f)(void *self, void *other);

typedef void (*cds_destruct_f)(void *self);

typedef int (*cds_cmp_f)(const void *self, const void *other);

#define _CDS_ASSERT(Expr, ModuleName, ErrFmt, ...) \
do { \
    if(!(Expr)) { \
        fprintf(stderr, "[%s on line %d in %s] " ErrFmt "\n", ModuleName, __LINE__, __func__, ##__VA_ARGS__); \
    } \
} while(0)

