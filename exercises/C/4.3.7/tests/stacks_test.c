#ifndef JQR_339_STACKS_TEST_H
#define JQR_339_STACKS_TEST_H

#include "../include/stacks.h"
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

#define TEST_STDOUT_CAPTURE_FILE "/tmp/CUnit_JQR_339_STACKS_TEST_stdout.log"

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

static int testStackContents(STACK_p_t stack, int *arr, int len)
{
    NODE_p_t current = stack->top;
    for (int index = 0; index < len; index++)
    {
        if ((current == NULL) || (current->data != arr[index]))
            return 0;
        current = current->next;
    }
    return 1;
}

void jqr_test_create_stack(void)
{
    printf("Inside create stack\n");
    STACK_p_t emptyStack = create_stack(NULL, 0);
    CU_ASSERT_PTR_NULL(emptyStack->top);
    destroy(emptyStack);
    printf("Inside create stack after empty destroyed\n");

    int test[] = {1, 2, 3, 4, 5};
    STACK_p_t popStack = create_stack(test, 5);
    CU_ASSERT_PTR_NOT_NULL(popStack->top);
    CU_ASSERT_EQUAL(testStackContents(popStack, test, 5), 1);
    destroy(popStack);
}

void jqr_test_push(void)
{
    printf("Inside push\n");
    STACK_p_t emptyStack = create_stack(NULL, 0);
    CU_ASSERT_PTR_NULL(emptyStack->top);
    push(emptyStack, 1);
    push(emptyStack, 2);
    push(emptyStack, 3);
    int test[] = {3, 2, 1};
    CU_ASSERT_PTR_NOT_NULL(emptyStack->top);
    CU_ASSERT_EQUAL(testStackContents(emptyStack, test, 3), 1);
    destroy(emptyStack);
}

void jqr_test_pop_at(void)
{
    printf("Inside pop at\n");
    int test[] = {1, 2, 3, 4, 5};
    STACK_p_t popStack = create_stack(test, 5);
    CU_ASSERT_PTR_NOT_NULL(popStack->top);
    NODE_p_t popped = pop_at(popStack, 2);
    printf("Popped: %d\n", popped->data);
    print_stack(popStack);
    CU_ASSERT_PTR_NOT_NULL(popped);
    CU_ASSERT_EQUAL(popped->data, 3);
    int test2[] = {1, 2, 4, 5};
    CU_ASSERT_EQUAL(testStackContents(popStack, test2, 4), 1);
    NODE_p_t popped2 = pop_at(popStack, -1);
    CU_ASSERT_PTR_NULL(popped2);
    NODE_p_t popped3 = pop_at(popStack, 10);
    CU_ASSERT_PTR_NULL(popped3);
    free(popped);
    free(popped2);
    free(popped3);
    destroy(popStack);
}

void jqr_test_pop(void)
{
    printf("Inside pop\n");
    STACK_p_t emptyStack = create_stack(NULL, 0);
    CU_ASSERT_PTR_NULL(emptyStack->top);
    NODE_p_t emptyPopped = pop(emptyStack);
    CU_ASSERT_PTR_NULL(emptyPopped);

    push(emptyStack, 5);
    CU_ASSERT_PTR_NOT_NULL(emptyStack->top);
    NODE_p_t popped = pop(emptyStack);
    CU_ASSERT_PTR_NOT_NULL(popped);
    CU_ASSERT_PTR_NULL(emptyStack->top);
    CU_ASSERT_EQUAL(popped->data, 5);
    free(popped);
    destroy(emptyStack);
}

void jqr_test_peek_at(void)
{
    printf("Inside peek at\n");
    int test[] = {1, 2, 3, 4, 5};
    STACK_p_t popStack = create_stack(test, 5);
    CU_ASSERT_PTR_NOT_NULL(popStack->top);
    NODE_p_t peeked = peek_at(popStack, 2);
    CU_ASSERT_PTR_NOT_NULL(peeked);
    CU_ASSERT_EQUAL(peeked->data, 3);
    CU_ASSERT_EQUAL(testStackContents(popStack, test, 5), 1);
    NODE_p_t peeked2 = peek_at(popStack, -1);
    CU_ASSERT_PTR_NULL(peeked2);
    NODE_p_t peeked3 = peek_at(popStack, 10);
    CU_ASSERT_PTR_NULL(peeked3);
    NODE_p_t peeked4 = peek_at(popStack, 2);
    CU_ASSERT_PTR_NOT_NULL(peeked);
    CU_ASSERT_EQUAL(peeked4->data, 3);
    destroy(popStack);
}

void jqr_test_peek(void)
{
    printf("Inside peek\n");
    STACK_p_t emptyStack = create_stack(NULL, 0);
    CU_ASSERT_PTR_NULL(emptyStack->top);
    NODE_p_t emptyPeek = peek(emptyStack);
    CU_ASSERT_PTR_NULL(emptyPeek);

    push(emptyStack, 5);
    CU_ASSERT_PTR_NOT_NULL(emptyStack->top);
    NODE_p_t peeked = peek(emptyStack);
    CU_ASSERT_PTR_NOT_NULL(peeked);
    CU_ASSERT_PTR_NOT_NULL(emptyStack->top);
    CU_ASSERT_EQUAL(peeked->data, 5);
    destroy(emptyStack);
}

void jqr_test_print_stack(void)
{
    printf("Inside print stack\n");
    STACK_p_t emptyStack = create_stack(NULL, 0);
    char *p_output = NULL;
    _redirect_stdout();
    print_stack(emptyStack);
    p_output = _capture_and_restore_stdout();
    CU_ASSERT_PTR_NOT_NULL_FATAL(p_output);
    CU_ASSERT_STRING_EQUAL(p_output, "Stack()\n");
    destroy(emptyStack);
    free(p_output);

    int test[] = {1, 2, 3, 4, 5};
    STACK_p_t testStack = create_stack(test, 5);
    _redirect_stdout();
    print_stack(testStack);
    p_output = _capture_and_restore_stdout();
    CU_ASSERT_PTR_NOT_NULL_FATAL(p_output);
    CU_ASSERT_STRING_EQUAL(p_output, "Stack(1, 2, 3, 4, 5)\n");
    destroy(testStack);
    free(p_output);
}

void jqr_test_clear(void)
{
    printf("Inside clear\n");
    int test[] = {1, 2, 3, 4, 5, 5};
    STACK_p_t testStack = create_stack(test, 6);
    CU_ASSERT_EQUAL(testStackContents(testStack, test, 6), 1);
    clear(testStack);
    CU_ASSERT_PTR_NULL(testStack->top);
    free(testStack);
}

#endif
