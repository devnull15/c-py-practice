#ifndef JQR_3110_POINTERS_TEST_H
#define JQR_3110_POINTERS_TEST_H

#include "../include/jqr_pointers.h"
#include <stdlib.h>
//#include <CUnit/CUnitCI.h>
#include <CUnit/CUnit.h>

/*
    Reference the CUnit Users Guide:
    http://cunit.sourceforge.net/doc/writing_tests.html#tests

    Step 1. Create the test cases
    Step 2. _add the test suite to the registry
    Step 3. _add the test to the suite.
*/

#define RAND_RANGE(lower, upper) ((rand() % (upper - lower + 1)) + lower)

static void calledFunc(int *acc)
{
    (*acc)++;
}

static int addition(int acc, int curr)
{
    return acc + curr;
}

static int multiplication(int acc, int curr)
{
    return acc * curr;
}

void jqr_test_fresh_int_pointer(void)
{
    int *fresh = fresh_int_pointer();
    CU_ASSERT_PTR_NULL(fresh);
}

void jqr_test_get_value(void)
{
    int value = rand();
    int value2 = get_value(&value);
    CU_ASSERT_EQUAL(value2, value);
}

void jqr_test_print_address(void)
{
    int fd;
    int value = 0;
    FILE *stdout_file;
    char read_buffer[20];
    char snprintf_buffer[20];

    /* This is a trick to reopen stdout and save to a file
     * runs the program which sends its output to stdout which
     * is now sent to the file, so that the file can be opened
     * and checked to see if the printed output was correct.
    */ 

    fflush(stdout);
    fd = dup(fileno(stdout));
    freopen("stdout.txt", "w", stdout);

    print_address(&value); // Execute the function sending stdout to stdout.txt

    fflush(stdout);
    dup2(fd, fileno(stdout));
    close(fd);
    clearerr(stdout);

    if(stdout_file = fopen("stdout.txt", "r"))
    {
        if(fread(read_buffer, 1, 20, stdout_file))
        {
            snprintf(snprintf_buffer, 20, "%p", (char *)&value);

            /* Compare the output of the print_address() function to what should be displayed */
            CU_ASSERT_EQUAL(0, strncmp(snprintf_buffer, read_buffer, strlen(snprintf_buffer)));
        }
        else
        {	
            perror("fread error");
        }	    
        fclose(stdout_file);
        unlink("stdout.txt");
    }
    else
    {
        perror("freopen error");
    }
}

void jqr_test_set_value(void)
{
    int initial = rand();
    int value = rand();
    set_value(&initial, value);
    CU_ASSERT_EQUAL(initial, value);
}

void jqr_test_call_func(void)
{
    //Track how many times function is called
    int called_func_acc = 0;
    int numToCall = RAND_RANGE(0, 100);
    for (int i = 0; i < numToCall; i++)
    {
        call_func((void *)calledFunc, (void *)&called_func_acc);
    }
    CU_ASSERT_EQUAL(called_func_acc, numToCall);
}

void jqr_test_fold_left(void)
{
    int arrLen = RAND_RANGE(0, 20);
    int *array = calloc(arrLen, sizeof(int));
    CU_ASSERT_PTR_NOT_NULL(array);
    int mulTot = 1;
    int addTot = 0;
    for (int i = 0; i < arrLen; i++)
    {
        int current = RAND_RANGE(0, 5);
        array[i] = current;
        mulTot *= current;
        addTot += current;
    }
    int addRes = fold_left(array, arrLen, addition, 0);
    CU_ASSERT_EQUAL(addRes, addTot);
    int mulRes = fold_left(array, arrLen, multiplication, 1);
    CU_ASSERT_EQUAL(mulRes, mulTot);
    free(array);
}

#endif
