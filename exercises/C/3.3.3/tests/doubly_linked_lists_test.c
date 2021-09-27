#ifndef JQR_333_DOUBLY_LINKED_LISTS_TEST_H
#define JQR_333_DOUBLY_LINKED_LISTS_TEST_H

#include "../include/doubly_linked_lists.h"
#include <stdlib.h>
#include <CUnit/CUnitCI.h>
#include <CUnit/CUnit.h>

/*
    Reference the CUnit Users Guide:
    http://cunit.sourceforge.net/doc/writing_tests.html#tests

    Step 1. Create the test cases
    Step 2. _add the test suite to the registry
    Step 3. _add the test to the suite.
*/

#define TEST_STDOUT_CAPTURE_FILE "/tmp/CUnit_JQR_333_DOUBLY_LINKED_LISTS_TEST_stdout.log"

extern volatile int g_arrays_fd_stdout;
static FILE *fp_stdout_file = NULL;
static int cur_stdout_fd;

// Tells if list contents match array contents
static int testListContents(DLL_p_t list, int *arr, int len)
{
    if (list->length != len)
        return 0;
    NODE_p_t current = list->head;
    for (int index = 0; index < len; index++)
    {
        if ((current == NULL) || (current->data != arr[index]))
            return 0;
        current = current->next;
    }
    return 1;
}

void jqr_test_create_list(void)
{
    // Check for empty list
    DLL_p_t emptyList = create_list(NULL, 0);
    CU_ASSERT_PTR_NULL_FATAL(emptyList->head);
    CU_ASSERT_EQUAL_FATAL(emptyList->length, 0);
    destroy(emptyList);

    // Check for populated list
    int test[] = {1, 2, 3, 4, 5};
    DLL_p_t popList = create_list(test, 5);
    CU_ASSERT_PTR_NOT_NULL_FATAL(popList->head);
    CU_ASSERT_EQUAL_FATAL(popList->length, 5);
    CU_ASSERT_EQUAL_FATAL(testListContents(popList, test, 5), 1);
    destroy(popList);
}

void jqr_test_insert_list_item(void)
{
    DLL_p_t list = malloc(sizeof(DLL_t));
    list->head = NULL;
    list->length = 0;

    insert_list_item(list, 1, 1);
    insert_list_item(list, 1, 0);
    insert_list_item(list, 2, -1);
    insert_list_item(list, 3, 1);
    insert_list_item(list, 2, 1);

    // List should be List(1,2,3)
    CU_ASSERT_PTR_NOT_NULL(list->head);
    int result[] = {1, 2, 3};
    CU_ASSERT_EQUAL(testListContents(list, result, 3), 1);

    destroy(list);
}

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

void jqr_test_print_list(void)
{
    DLL_p_t emptyList = create_list(NULL, 0);
    char *p_output = NULL;
    _redirect_stdout();
    print_list(emptyList);
    p_output = _capture_and_restore_stdout();
    CU_ASSERT_PTR_NOT_NULL_FATAL(p_output);
    CU_ASSERT_STRING_EQUAL(p_output, "List()\n");
    destroy(emptyList);
    free(p_output);

    int test[] = {1, 2, 3, 4, 5};
    DLL_p_t testList = create_list(test, 5);
    _redirect_stdout();
    print_list(testList);
    p_output = _capture_and_restore_stdout();
    CU_ASSERT_PTR_NOT_NULL_FATAL(p_output);
    CU_ASSERT_STRING_EQUAL(p_output, "List(1, 2, 3, 4, 5)\n");
    destroy(testList);
    free(p_output);
}

void jqr_test_find(void)
{
    int test[] = {1, 2, 3, 4, 5, 5};
    DLL_p_t testList = create_list(test, 6);
    CU_ASSERT_EQUAL(find(testList, 3), 2);
    CU_ASSERT_EQUAL(find(testList, 5), 4);
    CU_ASSERT_EQUAL(find(testList, 6), -1);
    destroy(testList);
}

int int_cmp(int lhs, int rhs)
{
    if (lhs == rhs)
        return 0;
    if (lhs < rhs)
        return -1;
    return 1;
}

void jqr_test_dll_sort(void)
{
    int unsorted[] = {5, 2, 3, 1, 4, 0, -1};
    DLL_p_t unsortedList = create_list(unsorted, 7);
    CU_ASSERT_EQUAL(testListContents(unsortedList, unsorted, 7), 1);
    int sorted[] = {-1, 0, 1, 2, 3, 4, 5};
    dll_sort(unsortedList, int_cmp);
    CU_ASSERT_EQUAL(testListContents(unsortedList, sorted, 7), 1);
    destroy(unsortedList);
}

void jqr_test_clear(void)
{
    int test[] = {1, 2, 3, 4, 5, 5};
    DLL_p_t testList = create_list(test, 6);
    CU_ASSERT_EQUAL(testListContents(testList, test, 6), 1);
    clear(testList);
    CU_ASSERT_PTR_NULL(testList->head);
    CU_ASSERT_EQUAL(testList->length, 0);
    free(testList);
}

#endif
