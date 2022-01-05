#ifndef JQR_316_ARGUMENTS_TEST_H
#define JQR_316_ARGUMENTS_TEST_H

#include "../include/args.h"

#include <CUnit/Basic.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define TEST_STDOUT_FILE "/tmp/CUnit_JQR_316_ARGS_TEST_stdout.log"

static int saved_stdout;
static fpos_t saved_stdout_pos;

static void redirect_stdout(void)
{
    fgetpos(stdout, &saved_stdout_pos);
    saved_stdout = dup(STDOUT_FILENO);
    freopen(TEST_STDOUT_FILE, "w+", stdout);
    setvbuf(stdout, NULL, _IONBF, 0);
}

static char* capture_and_restore_stdout(void)
{
    char* p_output = NULL;

    fseek(stdout, 0L, SEEK_END);
    uint64_t file_size = ftell(stdout);
    fseek(stdout, 0L, SEEK_SET);
    p_output = calloc(1, file_size + 1);
    fread(p_output, file_size, 1, stdout);

    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    clearerr(stdout);
    fsetpos(stdout, &saved_stdout_pos);

    return p_output;
}

void test_basic(void)
{
    int argc = 2;
    char** argv = calloc(argc, sizeof(char*));
    char** envp = calloc(argc, sizeof(char*));

    for (int i = 0; i < argc; i++)
    {
        argv[i] = calloc(1, 128);
        snprintf(argv[i], 128, "argument%d", i);
    }

    envp[0] = calloc(1, 128);
    snprintf(envp[0], 128, "environment_variable");

    redirect_stdout();
    size_t envp_length = my_main(argc, argv, envp);
    char* out = capture_and_restore_stdout();
    CU_ASSERT_EQUAL(envp_length, argc - 1);
    CU_ASSERT_STRING_EQUAL(out, "argument0\nargument1\n");
    
    free(envp[0]);
    free(envp);
    for (int i = 0; i < argc; i++)
    {
        free(argv[i]);
    }
    free(argv);
    free(out);
}

void test_empty_envp(void)
{
    int argc = 2;
    char** argv = calloc(argc, sizeof(char*));
    char** envp = calloc(argc, sizeof(char*));

    for (int i = 0; i < argc; i++)
    {
        argv[i] = calloc(1, 128);
        snprintf(argv[i], 128, "argument%d", i);
    }

    redirect_stdout();
    size_t envp_length = my_main(argc, argv, envp);
    char* out = capture_and_restore_stdout();
    CU_ASSERT_EQUAL(envp_length, 0);
    CU_ASSERT_STRING_EQUAL(out, "argument0\nargument1\n");

    free(envp);
    for (int i = 0; i < argc; i++)
    {
        free(argv[i]);
    }
    free(argv);
    free(out);
}

void test_argc_zero(void)
{
    int argc = 2;
    char** argv = calloc(argc, sizeof(char*));
    char** envp = calloc(argc, sizeof(char*));

    for (int i = 0; i < argc; i++)
    {
        argv[i] = calloc(1, 128);
        snprintf(argv[i], 128, "argument%d", i);
    }

    envp[0] = calloc(1, 128);
    snprintf(envp[0], 128, "environment_variable");

    redirect_stdout();
    size_t envp_length = my_main(0, argv, envp);
    char* out = capture_and_restore_stdout();
    CU_ASSERT_EQUAL(envp_length, argc - 1);
    CU_ASSERT_STRING_EQUAL(out, "");

    free(envp[0]);
    free(envp);
    for (int i = 0; i < argc; i++)
    {
        free(argv[i]);
    }
    free(argv);
    free(out);
}

#endif
