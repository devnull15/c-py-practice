#include "hash_func_test.h"
#include "../include/hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>


/*
 * @brief Tests basic strings and key values.
 */
void test_basic(void)
{
    uint32_t res0 = hash("test 1 test 2 test 3", 20);
    uint32_t ans0 = 1886615467;

    char *bigstr = calloc(1, 4096);
    memset(bigstr,'A',4096);
    uint32_t res1 = hash(bigstr, 4096);
    free(bigstr);
    bigstr = NULL;
    uint32_t ans1 = 266239;

    CU_ASSERT_EQUAL(res0,ans0);
    CU_ASSERT_EQUAL(res1,ans1);
}

/*
 * @brief Tests null pointer.
 */
void test_null(void)
{
    uint32_t result = hash(NULL, 0);
    CU_ASSERT_EQUAL(result,0);
}

void test_no_pad(void)
{
    uint32_t res1 = hash("this is a test", 14);
    uint32_t ans1 = 1180002662;

    uint32_t res2 = hash("Hello", 5);
    uint32_t ans2 = 660958807;

    CU_ASSERT_EQUAL(res1, ans1);
    CU_ASSERT_EQUAL(res2, ans2);
}
