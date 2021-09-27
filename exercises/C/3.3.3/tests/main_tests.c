#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>
#include "doubly_linked_lists_test.h"

int main(int argc, char *argv[])
{

    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }
    CU_pSuite dll_test_suite = CU_add_suite("dll_tests", NULL, NULL);

    if (NULL == dll_test_suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(dll_test_suite, "test of create_list()", jqr_test_create_list)) ||
        (NULL == CU_add_test(dll_test_suite, "test of insert_list_item()", jqr_test_insert_list_item)) ||
        (NULL == CU_add_test(dll_test_suite, "test of print_list()", jqr_test_print_list)) ||
        (NULL == CU_add_test(dll_test_suite, "test of find()", jqr_test_find)) ||
        (NULL == CU_add_test(dll_test_suite, "test of dll_sort()", jqr_test_dll_sort)) ||
        (NULL == CU_add_test(dll_test_suite, "test of clear()", jqr_test_clear)))
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
