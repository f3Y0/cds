#include <stdio.h>
#include <stdlib.h>

#include <setjmp.h>
#include <cmocka.h>

#include <cds/vec.h>

static int setup0(void **state) {
    cds_vec_t *vec = malloc(sizeof *vec);
    if(!vec) return EXIT_FAILURE;

    cds_vec_construct_with_capacity(vec, &CDS_TYPE_INT, 0);
    *state = vec;

    return EXIT_SUCCESS;
}

static int teardown(void **state) {
    cds_vec_t *vec = *state;

    cds_vec_destruct(vec);

    return EXIT_SUCCESS;
}

static void test_add(void **state) {
    cds_vec_t v;
    cds_vec_t *vec = &v;
    cds_vec_construct_with_capacity(vec, &CDS_TYPE_INT, 1);

    /* Test empty vec */

    for(int i = 0; i < 3; i++) cds_vec_add(vec, &i);

    for(int i = 0; i < 3; i++) {
        assert_int_equal(
            CDS_VEC_GET_AT_DEREF(vec, i, int),
            i
        );
    }

    cds_vec_destruct(vec);

    /* Test non-empty vec */

    cds_vec_construct_with_capacity(vec, &CDS_TYPE_INT, 1);

    for(int i = 0; i < 3; i++) cds_vec_add(vec, &i);

    for(int i = 0; i < 3; i++) {
        assert_int_equal(
            CDS_VEC_GET_AT_DEREF(vec, i, int),
            i
        );
    }

    cds_vec_destruct(vec);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_add, NULL, NULL),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

