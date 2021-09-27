#include "test_wrap_assert.h"
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * @brief File scope globals used for setjmp/longjmp in error handling during tests.
 * 
 */

/**
 * @brief This function replaces calls to assert in a candidate's implementation functions.
 * 
 */

#define ASSERT_ERROR 20001
static bool catch_assert = false;
static bool print_expected = false;
static jmp_buf bufferA = {0};

__attribute__((noreturn)) static void test_assert(char *expression)
{
    if (!catch_assert)
    {
        printf("Unexpected assertion: %s\n", expression);
        abort();
    }
    if (print_expected)
    {
        printf("Caught assertion: %s\n", expression);
    }

    catch_assert = false;
    longjmp(bufferA, ASSERT_ERROR);
}

__attribute__((constructor)) void set_catch_assert(void)
{
    printf("Catching assertions\n");
    if (NULL != &set_assertion_handler)
    {
        set_assertion_handler(test_assert);
    }
    else
    {
        printf("set_assertion_handler was not already defined. Unable to catch assertions\n");
        abort();
    }
    printf("Custom assertion handler registered\n");
}


int set_test_assert_buffer(void)
{
    if (catch_assert)
    {
        printf("Fatal error: Already expecting an assert\n");
        abort();
    }
    catch_assert = true;
    int result = setjmp(bufferA);

    return result;
}

bool expecting_assertion(void)
{
    return catch_assert;
}

jmp_buf * mark_expecting_assertion(bool _print_expected)
{
    if (catch_assert)
    {
        printf("Fatal error: mark_expecting_assertion called while already expecting an assertion\n");
        abort();
    }
    catch_assert = true;
    print_expected = _print_expected;
    return &bufferA;
}

void clear_expecting_assertion(void)
{
    if (!catch_assert)
    {
         printf("Fatal error: clear_expecting_assertion called while not expecting an assertion\n");
         abort();
    }
    catch_assert = false;
}
