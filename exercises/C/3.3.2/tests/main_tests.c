#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>
#include "LinkedList_test.c"

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
    if ((NULL == CU_add_test(pSuite, "Linked List new", test_linked_list_new)) ||
        (NULL == CU_add_test(pSuite, "Linked List delete", test_linked_list_delete)) ||
        (NULL == CU_add_test(pSuite, "Linked List size", test_linked_list_size)) ||
        (NULL == CU_add_test(pSuite, "Linked List append", test_linked_list_append)) ||
        (NULL == CU_add_test(pSuite, "Linked List append to Null list", test_linked_list_append_to_null)) ||
        (NULL == CU_add_test(pSuite, "Linked List find valid value", test_linked_list_find_with_valid_value)) ||
        (NULL == CU_add_test(pSuite, "Linked List find invalid value", test_linked_list_find_with_invalid_value)) ||
        (NULL == CU_add_test(pSuite, "Linked List find with NULL List", test_linked_list_find_Null_list)) ||
        (NULL == CU_add_test(pSuite, "Linked List get node's data pointer at invalid index", test_linked_list_at_invalid_index)) ||
        (NULL == CU_add_test(pSuite, "Linked List delete head", test_linked_list_del_head)) ||
        (NULL == CU_add_test(pSuite, "Linked List delete head from NULL list", test_linked_list_del_head_from_null_list)) ||
        (NULL == CU_add_test(pSuite, "Linked List delete head from EMPTY list", test_linked_list_del_head_from_empty_list)) ||
        (NULL == CU_add_test(pSuite, "Linked List delete node from list at valid index", test_linked_list_del_at_valid_index)) ||
        (NULL == CU_add_test(pSuite, "Linked List delete node at invalid index", test_linked_list_del_at_invalid_index)) ||
        (NULL == CU_add_test(pSuite, "Linked List for each node apply function", test_linked_list_for_each)) ||
        (NULL == CU_add_test(pSuite, "Linked List insert before", test_linked_list_insert_before)))
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
