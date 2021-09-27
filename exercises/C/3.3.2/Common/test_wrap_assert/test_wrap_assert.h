#ifndef TEST_WRAP_ASSERT_H
#define TEST_WRAP_ASSERT_H

#include <stdbool.h>
#include <setjmp.h>
//This file uses a custom attribute to inform clang tidy of control flow
#pragma GCC diagnostic ignored "-Wattributes"

/**
 * @brief typdef for a functin to be called when an assertion fails
 * 
 * @param expression[in] stringified assertion that failed
 */
typedef __attribute__((noreturn)) void (*assertion_handler_f)(char *expression);

/**
 * @brief sets the assertion handler to be called when an assertion fails
 * 
 * @param new_handler[in] will be called from now on whenever an assertion fails
 */
void set_assertion_handler(assertion_handler_f new_handler);

/**
 * @brief Called by each test case before calling a candidate-implemented function.
 *        Sets the jump environment buffer for longjmp to return to when test_assert is called.
 *        When candidate calls assert(), this has been remapped by the test suite to test_assert.
 *        DO NOT CALL THIS FUNCTION DIRECTLY 
 *        Use a wrapper macro defined later in this file.
 * 
 * @return opaque jmp_buf.  
 */
jmp_buf *mark_expecting_assertion(bool print_expected);

/**
 * @brief Clears the internal flag marking and expected assertion. Any assertions thown will now abort()
 */
void clear_expecting_assertion(void);

/**
 * @brief returns state of internal flag of if an assertion is expected
 * 
 * @returns state of internal flag of if an assertion is expected
 */
bool expecting_assertion(void);

/**
 * @brief Fatally fails a test suite if we are expecting an assertion. 
 * The internal state should always be reset in every function that uses it.
 */
static inline void verify_assertion_wrapper_state(void)
{
    CU_ASSERT_FALSE_FATAL(expecting_assertion());
}

// To give prettier failure messages
#define EXPECTED_ASSERTION_NOT_THROWN 0

// Will non-fatally fail a test if the expression does not throw an assertion
#define CU_ASSERT_ASSERTED(expression)                                                    \
    {                                                                                     \
        int set_result = setjmp((struct __jmp_buf_tag *)mark_expecting_assertion(false)); \
        if (0 != set_result)                                                              \
        {                                                                                 \
            CU_ASSERT_TRUE(true);                                                         \
        }                                                                                 \
        else                                                                              \
        {                                                                                 \
            (expression);                                                                 \
            clear_expecting_assertion();                                                  \
            CU_ASSERT_TRUE(EXPECTED_ASSERTION_NOT_THROWN);                                \
        }                                                                                 \
    }

// Called with __attribute__((cleanup)) on a dummy variable to reset expecting assertions to false
static inline void clear_expecting_assertion_on_exit(void **ignored)
{
    (void)ignored;
    clear_expecting_assertion();
}

// Causes an unexpected assertion to fatally fail the test, but not abort()
#define CU_CATCH_ASSERTIONS()                                                            \
    verify_assertion_wrapper_state();                                                    \
    {                                                                                    \
        int set_result = setjmp((struct __jmp_buf_tag *)mark_expecting_assertion(true)); \
        if (0 != set_result)                                                             \
        {                                                                                \
            CU_FAIL_FATAL("Unexpected Assertion");                                       \
        }                                                                                \
    }                                                                                    \
    void __attribute__((cleanup(clear_expecting_assertion_on_exit))) *_dummy = NULL;

// Pauses catching assertions so you can call CU_ASSERT_ASSERTED()
#define CU_PAUSE_CATCHING_ASSERTIONS() clear_expecting_assertion()

// Resumes catching assertions after a call to CU_PAUSE_CATCHING_ASSERTIONS()
#define CU_CONTINUE_CATCHING_ASSERTIONS()                                                \
    {                                                                                    \
        int set_result = setjmp((struct __jmp_buf_tag *)mark_expecting_assertion(true)); \
        if (0 != set_result)                                                             \
        {                                                                                \
            CU_FAIL_FATAL("Unexpected Assertion");                                       \
        }                                                                                \
    }

#endif
