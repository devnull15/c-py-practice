#ifndef JQR_318_FUNCTION_TEST_H
#define JQR_318_FUNCTION_TEST_H

#include "../include/functions.h"

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>
#include <stdlib.h>
#include <unistd.h>

#define TEST_STDIN_FILE "/tmp/CUnit_JQR_318_FUNC_TEST_stdin.log"

static int saved_stdin;

/**
 * @brief Internal helper function to redirect stdin to read from a file.
 * 
 */
static void _redirect_stdin(void)
{
    saved_stdin = dup(STDIN_FILENO);
    freopen(TEST_STDIN_FILE, "r", stdin);
    setvbuf(stdin, NULL, _IONBF, 0);
}

/**
 * @brief Internal helper function to restore the stdin fd.
 * 
 */
static void _restore_stdin(void)
{
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
}

/**
 * @brief Internal helper function to write the input to the file that we are
 * using for stdin.
 * 
 * @param input The input to write to the file
 * @return int The number of bytes written
 */
static int _write_input(const char* input)
{
    FILE *input_file = fopen(TEST_STDIN_FILE, "w");
    int wrote = fputs(input, input_file);
    fclose(input_file);
    return wrote;
}

static char _setup_run_teardown(char* input)
{
    int wrote = _write_input(input);

    _redirect_stdin();
    char out = get_char();
    _restore_stdin();

    return out;
}

void test_pbr_even(void)
{
    int val = 1;
    int timer = 20;
    pass_by_ref(timer, &val);
    CU_ASSERT_EQUAL(val, 0);
}

void test_pbr_odd(void)
{
    int val = 0;
    int timer = 21;
    pass_by_ref(timer, &val);
    CU_ASSERT_EQUAL(val, 1);
}

void test_deref(void)
{
    int value = 0xdeadbeef;
    int ret = dereference(&value);
    CU_ASSERT_EQUAL(ret, value);
}

void test_get_char(void)
{
    char *input = "a";
    char out = _setup_run_teardown(input);
    CU_ASSERT_EQUAL(out, 'a');
}

void test_fib(void)
{
    CU_ASSERT_EQUAL(fib_recursive(2), 1);
    CU_ASSERT_EQUAL(fib_recursive(9), 34);
}

/**
 * @brief Dummy function for testing.
 * 
 * @return int 1
 */
static int first(void)
{
    return 1;
}

/**
 * @brief Dummy function for testing.
 * 
 * @return int 2
 */
static int second(void)
{
    return 2;
}

void test_call(void)
{
    CU_ASSERT_EQUAL(call_my_function(first), 1);
    CU_ASSERT_EQUAL(call_my_function(second), 2);
}

#endif