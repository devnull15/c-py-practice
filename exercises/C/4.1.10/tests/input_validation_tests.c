#ifndef JQR_319_INPUT_VALIDATION_TEST_H
#define JQR_319_INPUT_VALIDATION_TEST_H

#include "../include/input_validation.h"
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


/**
 * @brief tests get_int() as an individual function.
 *
 */

void jqr_test_get_int(void)
{
    CU_ASSERT_EQUAL(valid_int("101"), 1);
    CU_ASSERT_EQUAL(valid_int("-101"), 1);
    CU_ASSERT_EQUAL(valid_int("a"), 0);
    CU_ASSERT_EQUAL(valid_int(""), 0);
    CU_ASSERT_EQUAL(valid_int("2147483647"), 1);
    CU_ASSERT_EQUAL(valid_int("-2147483647"), 1);
    CU_ASSERT_EQUAL(valid_int("2147483648"), 0);
    CU_ASSERT_EQUAL(valid_int("2147483647.0"), 0);
    CU_ASSERT_EQUAL(valid_int("\xFF"), 0);
    CU_ASSERT_EQUAL(valid_int("0x90"), 0);
    CU_ASSERT_EQUAL(valid_int("2-4-48-6-8"), 0);
}

/**
 * @brief tests get_char() as an individual function.
 *
 */

void jqr_test_get_char(void)
{
    CU_ASSERT_EQUAL(allowed_char("a"), 1);
    CU_ASSERT_EQUAL(allowed_char(""), 0);
    CU_ASSERT_EQUAL(allowed_char("AAA"), 0);
    CU_ASSERT_EQUAL(allowed_char("123f"), 1);
    CU_ASSERT_EQUAL(allowed_char("deadbeef"), 1);
    CU_ASSERT_EQUAL(allowed_char("0xdeadbeef"), 1);
    CU_ASSERT_EQUAL(allowed_char("0xdeadbeef!"), 0);
    CU_ASSERT_EQUAL(allowed_char("0xDEADBEEF"), 0);
}

#endif

