#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>
#include "stacks_test.h"

int main(int argc, char *argv[])
{

    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }
    CU_pSuite stacks_test_suite = CU_add_suite("stacks_tests", NULL, NULL);

    if (NULL == stacks_test_suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(stacks_test_suite, "test of create_stack()", jqr_test_create_stack)) ||
        (NULL == CU_add_test(stacks_test_suite, "test of push()", jqr_test_push)) ||
        (NULL == CU_add_test(stacks_test_suite, "test of pop_at()", jqr_test_pop_at)) ||
        (NULL == CU_add_test(stacks_test_suite, "test of pop()", jqr_test_pop)) ||
        (NULL == CU_add_test(stacks_test_suite, "test of peek()", jqr_test_peek)) ||
        (NULL == CU_add_test(stacks_test_suite, "test of peek_at()", jqr_test_peek_at)) ||
        (NULL == CU_add_test(stacks_test_suite, "test of clear()", jqr_test_clear)) ||
        (NULL == CU_add_test(stacks_test_suite, "test of print_stack()", jqr_test_print_stack)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);

    if (argc >= 1)
    {
        CU_set_output_filename(argv[0]);
    }

    CU_automated_enable_junit_xml(CU_TRUE);
    CU_automated_run_tests();
    CU_list_tests_to_file();

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
