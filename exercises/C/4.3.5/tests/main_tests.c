#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>
#include "bst_test.h"

int main(int argc, char *argv[])
{

    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }
    CU_pSuite bst_test_suite = CU_add_suite("bst_tests", NULL, NULL);

    if (NULL == bst_test_suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(bst_test_suite, "test of create_bst()", jqr_test_create_bst)) ||
        (NULL == CU_add_test(bst_test_suite, "test of insert()", jqr_test_insert_find)) ||
        (NULL == CU_add_test(bst_test_suite, "test of remove_node()", jqr_test_remove_node)) ||
        (NULL == CU_add_test(bst_test_suite, "test of print_bst()", jqr_test_print_bst)))
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
