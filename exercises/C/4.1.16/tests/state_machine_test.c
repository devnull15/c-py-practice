/**
 * @brief Test implementation for the state machine
 *
 */

#ifndef JQR_3115_SM_TESTS_H
#define JQR_3115_SM_TESTS_H

#include <CUnit/CUnit.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/bfe.h"

#define TEST_STDOUT_CAPTURE_FILE "/tmp/CUnit_JQR_3115_SM_TEST_stdout.log"
#define TEST_STDIN_FILE "/tmp/CUnit_JQR_3115_SM_TEST_stdin.log"
#define TEST_PROG_FILE "/tmp/CUnit_JQR_3115_SM_TEST_bfe"

static int    saved_stdout;
static fpos_t saved_stdout_pos;

static int saved_stdin;

/**
 * @brief Internal helper function to redirect stdout to a file.
 */
static void
_redirect_stdout()
{
    fgetpos(stdout, &saved_stdout_pos);
    saved_stdout = dup(STDOUT_FILENO);
    freopen(TEST_STDOUT_CAPTURE_FILE, "w+", stdout);
    setvbuf(stdout, NULL, _IONBF, 0);
}

/**
 * @brief Internal helper function to read stdout and then restore the stdout
 * fd.
 */
static char *
_capture_and_restore_stdout()
{
    char *p_output = NULL;

    // OK because we know that it's pointing to a file and files are seekable
    fseek(stdout, 0L, SEEK_END);
    uint64_t file_size = ftell(stdout);
    fseek(stdout, 0L, SEEK_SET);
    p_output = calloc(1, file_size + 1);
    fread(p_output, file_size, 1, stdout);

    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    clearerr(stdout);
    fsetpos(stdout, &saved_stdout_pos);

    if (NULL != p_output)
    {
#ifdef SHOW_OUTPUT
        printf("\n======Captured Output========\n%s======End Captured "
               "Output====\n",
               p_output);
#endif
    }

    return p_output;
}

/**
 * @brief Internal helper function to redirect stdin to read from a file.
 */
static void
_redirect_stdin()
{
    saved_stdin = dup(STDIN_FILENO);
    freopen(TEST_STDIN_FILE, "r", stdin);
    setvbuf(stdin, NULL, _IONBF, 0);
}

/**
 * @brief Internal helper function to restore the stdin fd.
 */
static void
_restore_stdin()
{
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
}

/**
 * @brief Internal helper function to write the command input to the file that
 * we are using for stdin.
 * @param input The input to write to the file
 * @param fname The filename to write to
 */
static int
_write_file(const char *input, const char *fname)
{
    FILE *input_file = fopen(fname, "w");
    int   wrote      = fputs(input, input_file);
    fclose(input_file);

    return wrote;
}

/**
 * @brief Internal helper function to redirect stdin/stdout, call the main
 * event loop. Captures the output of the run and then restores stdin/stdout.
 *
 * @param input The input for the specific test run
 * @return Pointer to the captured stdout
 */
static int
_setup_run_teardown(char **p_output)
{
    int ret;

    _redirect_stdin();
    _redirect_stdout();
    ret       = main_event_loop(TEST_PROG_FILE);
    *p_output = _capture_and_restore_stdout();
    _restore_stdin();

    return ret;
}

bool
test_hello_world()
{
    // Prints Hello World!
    char *hello_prog = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>--"
                       "-.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    char *p_output;
    int   ret;

    _write_file(hello_prog, TEST_PROG_FILE);

    ret = _setup_run_teardown(&p_output);
    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_STRING_EQUAL(p_output, "Hello World!\n");
    free(p_output);
    return true;
}

bool
test_rot13()
{
    char *in = "A1b2C3d4!";
    // Does rot13
    char *prog =
        "-,+[-[>>++++[>++++++++<-]<+<-[>+>+>-[>>>]<[[>+<-]>>+>]<<<<<-]]>>>[-]+>"
        "--[-[<->+++[-]]]<[++++++++++++<[>-[>+>>]>[+[<+>-]>+>>]<<<<<-]>>[<+>-]>"
        "[-[-<<[-]>>]<<[<<->>-]>>]<<[<<+>>-]]<[-]<.[-]<-,+]";
    char *expected = "N1o2P3q4!";
    char *p_output;
    int   ret;

    _write_file(prog, TEST_PROG_FILE);
    _write_file(in, TEST_STDIN_FILE);
    ret = _setup_run_teardown(&p_output);
    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_STRING_EQUAL(p_output, expected);
    free(p_output);
    return true;
}

bool
test_head()
{
    char *in = "aaa\nbbb\nccc\nd\ne\nf\ng\nh\ni\nj\nh\n";
    // Prints first 10 lines of input
    char *prog     = "+>>>>>>>>>>-[,+[-.----------[[-]>]<->]<]";
    char *expected = "aaa\nbbb\nccc\nd\ne\nf\ng\nh\ni\nj\n";
    char *p_output;
    int   ret;

    _write_file(prog, TEST_PROG_FILE);
    _write_file(in, TEST_STDIN_FILE);
    ret = _setup_run_teardown(&p_output);
    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_STRING_NOT_EQUAL(p_output, in);
    CU_ASSERT_STRING_EQUAL(p_output, expected);
    free(p_output);
    return true;
}

bool
test_unbalanced()
{
    char *prog;
    char *p_output;
    int   ret;

    prog = "[]]";
    _write_file(prog, TEST_PROG_FILE);
    ret = _setup_run_teardown(&p_output);
    CU_ASSERT_NOT_EQUAL(ret, 0);
    free(p_output);

    prog =
        "-,+[->>++++[>++++++++<-]<+<-[>+>+>-[>>>]<[[>+<-]>>+>]<<<<<-]]>>>[-]+>"
        "--[-[<->+++-]]]<[++++++++++++<[>-[>+>>]>[+[<+>-]>+>>]<<<<<-]>>[<+>-]>"
        "[-[-<<[-]>>]<<[<<->>-]>>]<<[<<+>>-]]<[-]<.[-]<-,+]";
    _write_file(prog, TEST_PROG_FILE);
    ret = _setup_run_teardown(&p_output);
    CU_ASSERT_NOT_EQUAL(ret, 0);
    free(p_output);

    return true;
}

bool
test_null_filename()
{
    int ret = main_event_loop(NULL);
    CU_ASSERT_NOT_EQUAL(ret, 0);
    return true;
}

bool
test_quine()
{
    char *prog =
        "->++>+++>+>+>+++>>>>>>>>>>>>>>>>>>>>+>+>++>+++>++>>+++>+>>>>>>>>>>>>>>"
        ">>>>>>>>>>>>>>>>>>>+>+>>+++>>+++>>>>>+++>+>>>>>>>>>++>+++>+++>+>>+++>>"
        ">+++>+>++>+++>>>+>+>++>+++>+>+>>+++>>>>>>>+>+>>>+>+>++>+++>+++>+>>+++>"
        ">>+++>+>++>+++>++>>+>+>++>+++>+>+>>+++>>>>>+++>+>>>>>++>+++>+++>+>>+++"
        ">>>+++>+>+++>+>>+++>>+++>>++[[>>+[>]++>++[<]<-]>+[>]<+<+++[<]<+]>+[>]+"
        "+++>++[[<++++++++++++++++>-]<+++++++++.<]\x1a";
    char *p_output;
    int ret;

    _write_file(prog, TEST_PROG_FILE);
    ret = _setup_run_teardown(&p_output);
    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_STRING_EQUAL(p_output, prog);
    free(p_output);
    return true;
}

void
test_3_1_15()
{
    CU_ASSERT(test_hello_world());
    CU_ASSERT(test_rot13());
    CU_ASSERT(test_head());
    CU_ASSERT(test_unbalanced());
    CU_ASSERT(test_null_filename());
    CU_ASSERT(test_quine());
}

#endif
