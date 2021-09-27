#ifndef JQR_317_FILE_IO_TEST_H
#define JQR_317_FILE_IO_TEST_H

#include "../include/file_io.h"
#include <stdlib.h>
#include <CUnit/CUnitCI.h>
#include <CUnit/CUnit.h>
#include <sys/stat.h>

/*
    Reference the CUnit Users Guide:
    http://cunit.sourceforge.net/doc/writing_tests.html#tests

    Step 1. Create the test cases
    Step 2. _add the test suite to the registry
    Step 3. _add the test to the suite.
*/

#define TEST_STDOUT_CAPTURE_FILE "/tmp/CUnit_JQR_317_FILE_IO_TEST_stdout.log"
#define DATA "The quick brown fox jumps over the lazy dog.\nThis I know to be the only true thing in this world."
#define APPEND_DATA "\nHe was quick and swift and could kick."
#define INSERT_DATA "But he was not as quick as he thought.\n"
#define FILENAME "tmp.txt"
#define INSERT_INDEX 45

static int    saved_stdout;
static fpos_t saved_stdout_pos;

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

    return p_output;
}


/**
 * @brief function to test if a file exists at a path
 * @param filename
 * @return 1 if file exists, 0 otherwise
 */

static int fileExists(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "r");
    int result = (NULL != fp);
    fclose(fp);
    return result;
}

/**
 * @brief reads the contents of a file into a buffer
 * @return 1 on success, 0 on failure
 */

static char *readFromFile(char *filename)
{
    FILE *fp = fopen(filename, "r");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buffer = malloc(sizeof(char) * (fsize + 1));
    if (NULL == buffer)
        return 0;
    fread(buffer, 1, fsize, fp);
    fclose(fp);
    buffer[fsize] = '\0';
    return buffer;
}

/**
 * @brief tests create_file() as an individual function. 
 * 
 */

void jqr_test_create_file(void)
{
    char filename[] = FILENAME;
    char data[] = DATA;
    int result = create_file(filename, data);
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_EQUAL_FATAL(fileExists(filename), 1);
    char *buffer = NULL;
    if (NULL != (buffer = readFromFile(filename)))
    {
        CU_ASSERT_STRING_EQUAL(data, buffer);
    }
    else
    {
        perror("Could not read contents of file into buffer.");
    }
    free(buffer);
    remove(filename);
}

/**
 * @brief tests modify_file() as an individual function
 * 
 */
void jqr_test_modify_file(void)
{
    char filename[] = FILENAME;
    char data[] = DATA;
    int datalen = strlen(data);
    char reverse[datalen];
    for (int i = 0; i < datalen; i++)
    {
        reverse[i] = data[(datalen - i) - 1];
    }
    reverse[datalen] = '\0';
    FILE *fp = fopen(filename, "w+");
    if (NULL == fp)
    {
        perror("Error opening file for testing");
        remove(filename);
        return;
    }
    fprintf(fp, "%s", data);
    fclose(fp);

    int result = modify_file(filename);
    CU_ASSERT_EQUAL(result, 1);
    char *buffer = NULL;
    if (NULL != (buffer = readFromFile(filename)))
    {
        CU_ASSERT_STRING_EQUAL(buffer, reverse);
    }
    else
    {
        perror("Could not read contents of file into buffer.");
    }
    free(buffer);
    remove(filename);
}

/**
 * @brief tests append_file() as an individual function
 * 
 */

void jqr_test_append_file(void)
{
    char filename[] = FILENAME;
    char data[] = DATA;
    char append_data[] = APPEND_DATA;
    int total_len = strlen(data) + strlen(append_data);
    char *concatted = malloc(sizeof(char) * (total_len + 1));
    strcpy(concatted, data);
    strcat(concatted, append_data);
    concatted[total_len] = '\0';

    FILE *fp = fopen(filename, "w+");
    if (NULL == fp)
    {
        remove(filename);
        perror("Error opening file for testing");
        return;
    }
    fprintf(fp, "%s", data);
    fclose(fp);

    int result = append_file(filename, append_data);
    CU_ASSERT_EQUAL(result, 1);

    char *buffer = NULL;
    if (NULL != (buffer = readFromFile(filename)))
    {
        CU_ASSERT_STRING_EQUAL(buffer, concatted);
    }
    else
    {
        perror("Could not read contents of file into buffer.");
    }
    free(concatted);
    free(buffer);
    remove(filename);
}

/**
 * @brief tests insert_file() as an individual function
 * 
 */
void jqr_test_insert_file(void)
{
    char filename[] = FILENAME;
    char data[] = DATA;
    int insert_index = INSERT_INDEX;
    char insert_data[] = INSERT_DATA;
    int total_len = strlen(data) + strlen(insert_data);
    char *concatted = calloc(total_len + 1, sizeof(char));
    strncpy(concatted, data, insert_index);
    strcat(concatted, insert_data);
    strncat(concatted, &data[insert_index], strlen(data) - insert_index);

    FILE *fp = fopen(filename, "w+");
    if (NULL == fp)
    {
        remove(filename);
        perror("Error opening file for testing");
        return;
    }
    fprintf(fp, "%s", data);
    fclose(fp);

    int result = insert_file(filename, insert_data, insert_index);
    CU_ASSERT_EQUAL(result, 1);

    char *buffer = NULL;
    if (NULL != (buffer = readFromFile(filename)))
    {
        CU_ASSERT_STRING_EQUAL(buffer, concatted);
        
    }
    else
    {
        perror("Could not read contents of file into buffer.");
    }
    free(concatted);
    free(buffer);
    remove(filename);
}

/**
 * @brief tests find_char_file() as an individual function
 * 
 */
void jqr_test_find_char_file(void)
{
    char filename[] = FILENAME;
    char data[] = DATA;
    FILE *fp = fopen(filename, "w+");
    if (NULL == fp)
    {
        perror("Error opening file for testing");
    }
    fprintf(fp, "%s", data);
    fclose(fp);
    CU_ASSERT_EQUAL(find_char_file(filename, 't'), 31);
    CU_ASSERT_EQUAL(find_char_file(filename, 'T'), 0);
    CU_ASSERT_EQUAL(find_char_file(filename, 'x'), 18);
    CU_ASSERT_EQUAL(find_char_file(filename, '?'), -1);
    remove(filename);
}

/**
 * @brief tests file_info() as an individual function
 * 
 */
void jqr_test_file_info(void)
{
    char fname[] = "/tmp/jqr_3_1_7_test_XXXXXX";
    char *dne = "/tmp/super_fake_file";
    char *contents = "this is a test\n";
    char *error = "Error retrieving statistics for /tmp/super_fake_file\n";
    char *res;
    char *expected;
    int tmp_fd;
    ssize_t written;

    tmp_fd = mkstemp(fname);
    if (-1 == tmp_fd)
    {
        CU_FAIL_FATAL("Could not create temporary file");
    }
    if (-1 == fchmod(tmp_fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH))
    {
        CU_FAIL_FATAL("Could not set file permissions");
    }
    written = write(tmp_fd, contents, 15);
    close(tmp_fd);

    // Just get something we know will be big enough
    expected = calloc(1, 256);
    snprintf(expected, 255,
            "Statistics for %s\nFile Size: %ld bytes\n"
            "File Permissions: -rw-r--r--\n", fname, written);

    _redirect_stdout();
    file_info(fname);
    res = _capture_and_restore_stdout();
    CU_ASSERT_STRING_EQUAL(expected, res);

    free(expected);
    free(res);

    // Also test something we know won't exist
    _redirect_stdout();
    file_info(dne);
    res = _capture_and_restore_stdout();
    CU_ASSERT_STRING_EQUAL(error, res);
    free(res);
}

/**
 * @brief tests delete_file() as an individual function
 * 
 */

void jqr_test_delete_file(void)
{
    char filename[] = FILENAME;
    FILE *fp = fopen(filename, "w+");
    if (NULL == fp)
    {
        perror("Error opening file for testing");
    }
    fclose(fp);
    int result = delete_file(filename);
    CU_ASSERT_EQUAL(result, 1);
    FILE *fp2 = fopen(filename, "r");
    CU_ASSERT_PTR_NULL(fp2);
}

#endif
