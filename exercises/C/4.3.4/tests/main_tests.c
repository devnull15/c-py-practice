#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>
#include "tree_test.c"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }

    CU_pSuite pSuite = CU_add_suite("graph_tests", NULL, NULL);

    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // 3. _add tests to the suite
    if ((NULL == CU_add_test(pSuite, "Tree New", test_tree_new)) ||
        (NULL == CU_add_test(pSuite, "Tree Add", test_tree_add)) ||
        (NULL == CU_add_test(pSuite, "Tree Add to Null Tree", test_tree_add_to_null)) ||
        (NULL == CU_add_test(pSuite, "Tree Find", test_tree_find)) ||
        (NULL == CU_add_test(pSuite, "Tree Find Nothing", test_tree_find_nothing)) ||
        (NULL == CU_add_test(pSuite, "Tree Find No Tree", test_tree_find_no_tree)) ||
        (NULL == CU_add_test(pSuite, "Tree Remove head", test_tree_rm_head)) ||
        (NULL == CU_add_test(pSuite, "Tree Remove head from NULL tree", test_tree_rm_head_from_null)) ||
        (NULL == CU_add_test(pSuite, "Tree Remove head no nodes", test_tree_rm_head_no_nodes)) ||
        (NULL == CU_add_test(pSuite, "Tree Destroy", test_tree_destroy)) ||
        (NULL == CU_add_test(pSuite, "Tree and not a list", test_is_tree_not_list)) ||
        (NULL == CU_add_test(pSuite, "Tree Remove head to empty", test_tree_rm_head_to_empty)))
    {
        CU_cleanup_registry();
        CU_get_error();
        return;
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
