#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <CUnit/CUnitCI.h>

#include "hash_table_test.h"

int init(void) { return 0; }
int clean(void) { return 0; }

int main(int argc, char **argv) {
    // Init CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);

    CU_TestInfo tests[] = {
        {"create", test_create},
        {"create null hash function", test_create_null_func},
        {"null table insert", test_insert_null_table},
        {"insert realloc", test_insert_realloc},
        {"insert", test_insert},
        {"insert same key", test_insert_same},
        {"null table delete", test_delete_null_table},
        {"delete key dne", test_delete_key_dne},
        {"delete", test_delete},
        {"get table null", test_get_null_table},
        {"get item dne", test_get_item_dne},
        {"get", test_get},
        {"empty null table", test_empty_null},
        {"empty", test_empty},
        {"destroy null table", test_destroy_null},
        {"destroy", test_destroy},
        CU_TEST_INFO_NULL,
    };

    CU_SuiteInfo suites[] = {
        {"hash_table test suite", init, clean, NULL, NULL, tests},
        CU_SUITE_INFO_NULL,
    };

    if (CUE_SUCCESS != CU_register_suites(suites)) {
        fprintf(stderr, "Register suites failed - %s ", CU_get_error_msg());
        exit(1);
    }

    if (argc >= 1) {
        CU_set_output_filename(argv[0]);
    }

    CU_automated_enable_junit_xml(CU_TRUE);
    CU_automated_run_tests();
    CU_list_tests_to_file();
    
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
