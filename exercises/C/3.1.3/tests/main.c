#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <CUnit/Automated.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "test_data_types.h"

int main(int argc, char* argv[])
{
    if(CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite data_type_suite = CU_add_suite("data_type_tests", NULL, NULL);

    srand(time(NULL));

    if (NULL == data_type_suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(data_type_suite, "Section 3.1.3", test_3_1_3))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);

    if (argc >= 1)
    {
        printf("%s\n", argv[0]);
        CU_set_output_filename(argv[0]);
    }

    CU_automated_enable_junit_xml(CU_TRUE);
    CU_automated_run_tests();
    CU_list_tests_to_file();

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
