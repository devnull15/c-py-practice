#ifndef JQR_352_INTERRUPTS_TEST_H
#define JQR_352_INTERRUPTS_TEST_H

#include "CUnit/Basic.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>

#include "../include/interrupts.h"

#define TEST_STDOUT_CAPTURE_FILE "/tmp/CUnit_JQR_352_INT_TEST_stdout.log"

static int saved_stdout;
static fpos_t saved_stdout_pos;

static volatile sig_atomic_t caught_trap = 0;

static void redirect_stdout()
{
    fgetpos(stdout, &saved_stdout_pos);
    saved_stdout = dup(STDOUT_FILENO);
    freopen(TEST_STDOUT_CAPTURE_FILE, "w+", stdout);
    setvbuf(stdout, NULL, _IONBF, 0);
}

static char* capture_and_restore_stdout()
{
    char* p_output = NULL;
    uint64_t file_size;

    fseek(stdout, 0L, SEEK_END);
    file_size = ftell(stdout);
    fseek(stdout, 0L, SEEK_SET);
    p_output = calloc(1, file_size + 1);
    fread(p_output, file_size, 1, stdout);

    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    clearerr(stdout);
    fsetpos(stdout, &saved_stdout_pos);

    return p_output;
}

void test_syscall(void)
{
    char* p_output = NULL;
    char* msg = "hello";

    redirect_stdout();
    CU_ASSERT_EQUAL_FATAL(do_write(msg), strlen(msg));
    p_output = capture_and_restore_stdout();
    CU_ASSERT_PTR_NOT_NULL_FATAL(p_output);
    CU_ASSERT_EQUAL(memcmp(p_output, msg, strlen(msg)), 0);
    free(p_output);
}

static void sigtrap_handler(int signal)
{
    if (signal == SIGTRAP)
    {
        caught_trap++;
    }
}

void test_int3(void)
{
    signal(SIGTRAP, sigtrap_handler); 
    caught_trap = 0;
    do_int3();
    CU_ASSERT_EQUAL(caught_trap, 1);
}

#endif
