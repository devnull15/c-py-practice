#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>
#include "input_validation_tests.h"

int main(int argc, char *argv[])
{

    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }
    CU_pSuite file_io_test_suite = CU_add_suite("input_validation_tests", NULL, NULL);

    if (NULL == file_io_test_suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(file_io_test_suite, "test of get_int()", jqr_test_get_int)) ||
        (NULL == CU_add_test(file_io_test_suite, "test of get_char()", jqr_test_get_char)))
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
