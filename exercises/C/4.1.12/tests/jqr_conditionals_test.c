#ifndef JQR_3111_CONDITIONALS_TEST_H
#define JQR_3111_CONDITIONALS_TEST_H

#include "../include/jqr_conditionals.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <CUnit/CUnitCI.h>
#include <CUnit/CUnit.h>
/*
    Reference the CUnit Users Guide:
    http://cunit.sourceforge.net/doc/writing_tests.html#tests

    Step 1. Create the test cases
    Step 2. _add the test suite to the registry
    Step 3. _add the test to the suite.
*/

#define TEST_STDOUT_CAPTURE_FILE "/tmp/CUnit_JQR_3111_CONDITIONALS_TEST_stdout.log"
#define RAND_RANGE(lower, upper) ((rand() % (upper - lower + 1)) + lower)

static FILE *fp_stdout_file = NULL;
static int cur_stdout_fd;

static node *prepend(node *head, int value)
{
    node *n = malloc(sizeof(node));
    n->data = value;
    n->next = head;
    return n;
}

static int mod2(int value)
{
    return value % 2;
}

static bool array_equal(int arr1[], int arr2[], int arrLen)
{
    for (int i = 0; i < arrLen; i++)
    {
        if (arr1[i] != arr2[i])
            return false;
    }
    return true;
}

void jqr_test_array_map(void)
{
    printf("Inside test array map\n");
    int arrLen = RAND_RANGE(1, 10);
    int *array = calloc(arrLen, sizeof(int));
    int *mapped = calloc(arrLen, sizeof(int));
    for (int i = 0; i < arrLen; i++)
    {
        array[i] = RAND_RANGE(0, 50);
        mapped[i] = mod2(array[i]);
    }
    int *result = array_map(array, arrLen, mod2);
    CU_ASSERT_PTR_NOT_NULL(result);
    bool sameArr = array_equal(mapped, result, arrLen);
    CU_ASSERT_TRUE(sameArr);
    free(array);
    free(mapped);
    free(result);
}

void jqr_test_sum_list(void)
{
    printf("Inside test sum list\n");
    int arrLen = RAND_RANGE(1, 10);
    node *head = NULL;
    int sumTotal = 0;
    for (int i = 0; i < arrLen; i++)
    {
        int current = RAND_RANGE(0, 50);
        sumTotal += current;
        if (!head)
        {
            head = malloc(sizeof(node));
            head->data = current;
            head->next = NULL;
        }
        else
        {
            head = prepend(head, current);
        }
    }
    int result = sum_list(head);
    CU_ASSERT_EQUAL(result, sumTotal);

    node *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void jqr_test_index_of_larger(void)
{
    printf("Inside test index of larger\n");
    int arrLen = RAND_RANGE(1, 10);
    int *array = calloc(arrLen, sizeof(int));
    int numToCompare = RAND_RANGE(0, 50);
    bool flag = false;
    int largerIndex = -1;
    for (int i = 0; i < arrLen; i++)
    {
        array[i] = RAND_RANGE(0, 50);
        if (array[i] > numToCompare && !flag)
        {
            largerIndex = i;
            flag = true;
        }
    }
    int result = index_of_larger(array, arrLen, numToCompare);
    CU_ASSERT_EQUAL(result, largerIndex);
    free(array);
}

void jqr_test_reckoner(void)
{
    printf("Inside test reckoner\n");
    char input[200];
    int truth = 0;
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567890!@#$%^&*()_+-=`~[]{}|,./<>?\\ ";
    ulong i;

    // Create random string of 200 chars

    for (i = 0; i < 200; ++i)
    {
        input[i] = charset[rand() % (sizeof(charset) - 1)];
    }

    input[i] = 0;  // Terminate string

    for (int i = 0 ; i < strlen(input) ; i++)
    {
        if (isalpha(input[i]))
        {
            truth += 1;
        }
        else if (isdigit(input[i]))
        {
            truth -= 1;
        }
        else if (isspace(input[i]))
        {
            truth -= 2;
        }
        else if (ispunct(input[i]))
        {
            truth += 2;
        }
    }

    int result = reckoner(input);
    CU_ASSERT_EQUAL(result, truth);
}

void jqr_test_isEvenOdd(void)
{
    printf("Inside test isEvenOdd\n");
    int value = RAND_RANGE(0, 1000);
    int truth = (value % 2 == 0) ? 0 : 1;
    int result = isEvenOdd(value);
    CU_ASSERT_EQUAL(result, truth);
}

void jqr_test_startsWith(void)
{
    printf("Inside test startsWith\n");
    char choices[] = {'a', 'b', 'c', 'd', 'e', 'f', 'd', 'h'};
    int index = RAND_RANGE(0, 7);
    char choice = choices[index];
    int truth;
    if (choice == 'a')
        truth = 1;
    else if (choice == 'b')
        truth = 2;
    else if (choice == 'c')
        truth = 3;
    else
        truth = -1;
    char input[2];
    input[0] = choice;
    input[1] = '\n';
    int result = startsWith(input);
    CU_ASSERT_EQUAL(result, truth);
}

void *xmalloc(size_t size)
{
    static int calls = 0;
    if (++calls > 1)
    {
        return NULL;
    }
    return malloc(size);
}

void jqr_test_demo_teardown(void)
{
    printf("Inside test demo_teardown\n");
    CU_ASSERT_NOT_EQUAL(0, demo_teardown());
    CU_PASS("Manual review of demo_teardown() required");
}

#endif
