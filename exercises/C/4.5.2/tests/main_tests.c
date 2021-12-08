#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>

#include "signal_test.h"
#include "interrupt_test.h"

int
init(void)
{
    return 0;
}

int
clean(void)
{
    return 0;
}

int
main(int argc, char **argv)
{
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);

    CU_TestInfo signals[] = {
        {"signal setup", test_signal_setup},
        {"signal handling", test_signal_handler},
        CU_TEST_INFO_NULL,
    };

    CU_TestInfo ints[] = {
        {"test sys_write", test_syscall},
        {"test int3", test_int3},
        CU_TEST_INFO_NULL,
    };

    CU_SuiteInfo suites[] = {
        {"signal test suite", init, clean, NULL, NULL, signals},
        {"interrupt test suite", init, clean, NULL, NULL, ints},
        CU_SUITE_INFO_NULL,
    };

    if (CUE_SUCCESS != CU_register_suites(suites))
    {
        fprintf(stderr, "register suites failed - %s ", CU_get_error_msg());
        exit(1);
    }

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