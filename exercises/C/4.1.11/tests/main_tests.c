#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
//#include <CUnit/CUnitCI.h>
#include "jqr_pointers_test.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }
    CU_pSuite jqr_pointers_test_suite = CU_add_suite("jqr_pointers_tests", NULL, NULL);

    if (NULL == jqr_pointers_test_suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(jqr_pointers_test_suite, "test of fresh_int_pointer()", jqr_test_fresh_int_pointer)) ||
        (NULL == CU_add_test(jqr_pointers_test_suite, "test of get_value()", jqr_test_get_value)) ||
        (NULL == CU_add_test(jqr_pointers_test_suite, "test of print_address()", jqr_test_print_address)) ||
        (NULL == CU_add_test(jqr_pointers_test_suite, "test of set_value()", jqr_test_set_value)) ||
        (NULL == CU_add_test(jqr_pointers_test_suite, "test of call_func()", jqr_test_call_func)) ||
        (NULL == CU_add_test(jqr_pointers_test_suite, "test of fold_left()", jqr_test_fold_left)))
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
