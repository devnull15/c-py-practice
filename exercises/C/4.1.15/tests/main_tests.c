#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "test.h"
#include "../include/sort.h"

int main(int argc, char* argv[])
{
    if(CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }

    //Initialize the CUnit test registry
    CU_pSuite sort_suite = NULL;
    sort_suite = CU_add_suite("SORT_TEST_suite", NULL, NULL);
    
    srand(time(NULL));

    //Add a suite to the registry
    if (NULL == sort_suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    //Add the tests to the suite
    //Note - Order is important for multiple file
    if (NULL == CU_add_test(sort_suite, "Section 3.1.14", 
    test_3_1_14))
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

    //Run tests
    CU_automated_enable_junit_xml(CU_TRUE);
    CU_automated_run_tests();
    CU_list_tests_to_file();

    CU_basic_run_tests();

    //Cleanup registry and return
    CU_cleanup_registry();
    return CU_get_error();

}