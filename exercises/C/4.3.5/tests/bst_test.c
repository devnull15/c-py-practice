#ifndef JQR_337_BST_TEST_H
#define JQR_337_BST_TEST_H

#include "../include/bst.h"
#include <stdlib.h>
#include <time.h>
#include <CUnit/CUnitCI.h>
#include <CUnit/CUnit.h>

/*
    Reference the CUnit Users Guide:
    http://cunit.sourceforge.net/doc/writing_tests.html#tests

    Step 1. Create the test cases
    Step 2. _add the test suite to the registry
    Step 3. _add the test to the suite.
*/

#define TEST_STDOUT_CAPTURE_FILE "/tmp/CUnit_JQR_337_BST_TEST_stdout.log"

extern volatile int g_arrays_fd_stdout;
static FILE *fp_stdout_file = NULL;
static int cur_stdout_fd;

static void _redirect_stdout()
{
    cur_stdout_fd = g_arrays_fd_stdout;
    fp_stdout_file = fopen(TEST_STDOUT_CAPTURE_FILE, "w+");

    if (NULL != fp_stdout_file)
    {
        g_arrays_fd_stdout = fileno(fp_stdout_file);
    }
}

static char *_capture_and_restore_stdout()
{
    char *p_output = NULL;
    g_arrays_fd_stdout = cur_stdout_fd;

    if (NULL != fp_stdout_file)
    {
        fseek(fp_stdout_file, 0L, SEEK_END);
        uint64_t file_size = ftell(fp_stdout_file);

        fseek(fp_stdout_file, 0, SEEK_SET);

        p_output = calloc(1, file_size + 1);
        fread(p_output, file_size, 1, fp_stdout_file);

        fclose(fp_stdout_file);
    }
    fp_stdout_file = NULL;
    if (NULL != p_output)
    {
#ifdef SHOW_OUTPUT
        printf("\n======Captured Output========\n%s======End Captured Output====\n", p_output);
#endif
    }

    return p_output;
}

void jqr_test_create_bst(void)
{
    BST_p_t emptyBST = create_bst(NULL, 0);
    CU_ASSERT_PTR_NULL(emptyBST->root);
    destroy(emptyBST);

    int test[] = {3, 1, 2, 4, 5};
    BST_p_t testBST = create_bst(test, 5);
    CU_ASSERT_PTR_NOT_NULL(testBST->root);
    destroy(testBST);
}

void jqr_test_insert_find(void)
{
    BST_p_t bst;
    NODE_p_t res;
    int val;
    int num_items;

    srand(time(0));

    bst = create_bst(NULL, 0);
    res = find(bst, 5);
    CU_ASSERT_PTR_NULL(bst->root);
    CU_ASSERT_PTR_NULL(res);

    // Just so it isn't too big.
    num_items = rand() % 10000;
    for (int i = 0; i < num_items; ++i)
    {
        val = rand();
        insert(bst, val);
        res = find(bst, val);
        CU_ASSERT_PTR_NOT_NULL_FATAL(res);
        CU_ASSERT_EQUAL(res->data, val);
    }
    destroy(bst);
}

void jqr_test_remove_node(void)
{
    BST_p_t bst;
    NODE_p_t node = NULL;
    int *inserted;
    int i;
    int val;
    int num_items;

    srand(time(0));
    bst = create_bst(NULL, 0);
    CU_ASSERT_PTR_NOT_NULL_FATAL(bst);

    num_items = rand() % 10000;
    inserted = calloc(num_items, sizeof(int));
    CU_ASSERT_PTR_NOT_NULL_FATAL(inserted);
    for (i = 0; i < num_items; ++i)
    {
        val = rand();
        inserted[i] = val;
        insert(bst, val);
    }
    for (i = 0; i < num_items; ++i)
    {
        val = inserted[i];
        remove_node(bst, val);
        node = find(bst, val);
        CU_ASSERT_PTR_NULL(node);
    }
    CU_ASSERT_PTR_NULL(bst->root);

    destroy(bst);
    free(inserted);
}

void jqr_test_print_bst(void)
{
    BST_p_t emptyBST = create_bst(NULL, 0);
    char *p_output = NULL;
    _redirect_stdout();
    print_bst(emptyBST);
    p_output = _capture_and_restore_stdout();
    CU_ASSERT_PTR_NOT_NULL_FATAL(p_output);
    CU_ASSERT_STRING_EQUAL(p_output, "BST()\n");
    destroy(emptyBST);
    free(p_output);

    int test[] = {3, 1, 2, 4, 5};
    BST_p_t testBST = create_bst(test, 5);
    _redirect_stdout();
    print_bst(testBST);
    p_output = _capture_and_restore_stdout();
    CU_ASSERT_PTR_NOT_NULL_FATAL(p_output);
    CU_ASSERT_STRING_EQUAL(p_output, "BST(1, 2, 3, 4, 5, )\n");
    destroy(testBST);
    free(p_output);
}

void jqr_test_clear(void)
{
    int test[] = {1, 2, 3, 4, 5, 6};
    BST_p_t testBST = create_bst(test, 6);
    CU_ASSERT_PTR_NOT_NULL(testBST->root);
    clear(testBST);
    CU_ASSERT_PTR_NOT_NULL(testBST);

    char *p_output = NULL;
    _redirect_stdout();
    print_bst(testBST);
    p_output = _capture_and_restore_stdout();
    CU_ASSERT_PTR_NOT_NULL_FATAL(p_output);
    CU_ASSERT_STRING_EQUAL(p_output, "BST()\n");
    destroy(testBST);
    free(p_output);
}

#endif
