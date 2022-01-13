#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>
#include "jqr_conditionals_test.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }
    CU_pSuite jqr_conditionals_test_suite = CU_add_suite("jqr_conditionals_tests", NULL, NULL);

    if (NULL == jqr_conditionals_test_suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(jqr_conditionals_test_suite, "test of array_map()", jqr_test_array_map)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of index_of_larger()", jqr_test_index_of_larger)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of sum_list()", jqr_test_sum_list)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of isEvenOdd()", jqr_test_isEvenOdd)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of reckoner()", jqr_test_reckoner)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of startsWith()", jqr_test_startsWith)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of demo_teardown()", jqr_test_demo_teardown)))
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
