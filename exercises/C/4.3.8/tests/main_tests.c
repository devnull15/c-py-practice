#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>
#include "graph_test.c"

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

    if ((NULL == CU_add_test(pSuite, "Graph new", test_graph_new)) ||
        (NULL == CU_add_test(pSuite, "Graph add a node", test_graph_add_node)) ||
        (NULL == CU_add_test(pSuite, "Graph add node to null", test_graph_add_null)) ||
        (NULL == CU_add_test(pSuite, "Graph add null node", test_graph_add_null_node)) ||
        (NULL == CU_add_test(pSuite, "Graph add edge", test_graph_add_edge)) ||
        (NULL == CU_add_test(pSuite, "Graph add edge to null", test_graph_add_edge_null)) ||
        (NULL == CU_add_test(pSuite, "Graph add edge from null node", test_graph_add_edge_null_src)) ||
        (NULL == CU_add_test(pSuite, "Graph add edge to null node", test_graph_add_edge_null_dst)) ||
        (NULL == CU_add_test(pSuite, "Graph find", test_graph_find)) ||
        (NULL == CU_add_test(pSuite, "Graph find in null", test_graph_find_null)) ||
        (NULL == CU_add_test(pSuite, "Graph find null node", test_graph_find_null_node)) ||
        (NULL == CU_add_test(pSuite, "Graph remove node", test_graph_rm_node)) ||
        (NULL == CU_add_test(pSuite, "Graph remove node from null", test_graph_rm_null_node)) ||
        (NULL == CU_add_test(pSuite, "Graph remove edge", test_graph_rm_edge)) ||
        (NULL == CU_add_test(pSuite, "Graph remove edge from null", test_graph_rm_edge_null)) ||
        (NULL == CU_add_test(pSuite, "Graph remove edge from null object", test_graph_rm_edge_null_src)) ||
        (NULL == CU_add_test(pSuite, "Graph remove edge to null object", test_graph_rm_edge_null_dst)) ||
        (NULL == CU_add_test(pSuite, "Graph destroy", test_graph_destroy)) ||
        (NULL == CU_add_test(pSuite, "Graph destroy null", test_graph_destroy_null)))
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
