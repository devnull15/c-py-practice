#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>
#include "queues_test.h"

int main(int argc, char *argv[])
{

    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }
    CU_pSuite queues_test_suite = CU_add_suite("queues_tests", NULL, NULL);
    CU_pSuite priority_queues_test_suite = CU_add_suite("priority_queues_tests", NULL, NULL);

    if (NULL == queues_test_suite || NULL == priority_queues_test_suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(queues_test_suite, "test of create_queue()", jqr_test_create_queue)) ||
        (NULL == CU_add_test(queues_test_suite, "test of enqueue()", jqr_test_enqueue)) ||
        (NULL == CU_add_test(queues_test_suite, "test of dequeue()", jqr_test_dequeue)) ||
        (NULL == CU_add_test(queues_test_suite, "test of peek()", jqr_test_peek)) ||
        (NULL == CU_add_test(queues_test_suite, "test of clear()", jqr_test_clear)) ||
        (NULL == CU_add_test(queues_test_suite, "test print node in queue", jqr_test_print_queue)) ||
        (NULL == CU_add_test(priority_queues_test_suite, "test of create_p_queue()", jqr_test_create_p_queue)) ||
        (NULL == CU_add_test(priority_queues_test_suite, "test of p_enqueue()", jqr_test_p_enqueue)) ||
        (NULL == CU_add_test(priority_queues_test_suite, "test of p_dequeue()", jqr_test_p_dequeue)) ||
        (NULL == CU_add_test(priority_queues_test_suite, "test of p_peek()", jqr_test_p_peek)) ||
        (NULL == CU_add_test(priority_queues_test_suite, "test of p_clear()", jqr_test_p_clear)))
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
