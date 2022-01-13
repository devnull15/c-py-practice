#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>
#include "cll_test.c"

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
    if ((NULL == CU_add_test(pSuite, "Circularly Linked List new", test_circularly_llist_new)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List add", test_circularly_llist_add)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List circular", test_circularly_llist_is_circular)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List add to Null list", test_circularly_llist_add_to_null)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List find", test_circularly_llist_find)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List find Nothing", test_circularly_llist_find_nothing)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List find No List", test_circularly_llist_find_no_list)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List find No List", test_circularly_llist_find_no_list)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List Remove head", test_circularly_llist_rm_head)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List Remove head from NULL list", test_circularly_llist_rm_head_from_null)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List Remove head no nodes", test_circularly_llist_rm_head_no_nodes)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List Remove tail", test_circularly_llist_rm_tail)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List Remove tail from NULL list", test_circularly_llist_rm_tail_from_null)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List Remove tail no nodes", test_circularly_llist_rm_tail_no_nodes)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List Remove node from list", test_circularly_llist_rm)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List destroy", test_circularly_llist_destroy)) ||
        (NULL == CU_add_test(pSuite, "Circularly Linked List Remove head from list of one node.", test_circularly_llist_rm_head_single_node)))
    {
        CU_cleanup_registry();
        CU_get_error();
        return -1;
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
