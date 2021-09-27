#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>
#include "file_io_test.h"

int main(int argc, char *argv[])
{

    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }
    CU_pSuite file_io_test_suite = CU_add_suite("file_io_tests", NULL, NULL);

    if (NULL == file_io_test_suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(file_io_test_suite, "test of create_file()", jqr_test_create_file)) ||
        (NULL == CU_add_test(file_io_test_suite, "test of modify_file()", jqr_test_modify_file)) ||
        (NULL == CU_add_test(file_io_test_suite, "test of append_file()", jqr_test_append_file)) ||
        (NULL == CU_add_test(file_io_test_suite, "test of insert_file()", jqr_test_insert_file)) ||
        (NULL == CU_add_test(file_io_test_suite, "test of find_char_file()", jqr_test_find_char_file)) ||
        (NULL == CU_add_test(file_io_test_suite, "test of file_info()", jqr_test_file_info)) ||
        (NULL == CU_add_test(file_io_test_suite, "test of delete_file()", jqr_test_delete_file)))
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