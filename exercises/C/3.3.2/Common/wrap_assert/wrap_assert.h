#ifndef WRAP_ASSERT_H_
#define WRAP_ASSERT_H_
#ifdef assert
#undef assert
#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

/**
 * @brief typdef for a functin to be called when an assertion fails
 * 
 * @param expression[in] stringified assertion that failed
 */
typedef __attribute__((noreturn)) void (*assertion_handler_f)(char *expression);

/**
 * @brief wrapper for the assertion handler. Will call whichever
 * handler was last installed with set_assertion_handler(), or
 * a default handler if none have been installed
 * 
 * @param expression[in] passed to wrapped assertion handler
 */
__attribute__((noreturn)) void test_assert(char *expression);

/**
 * @brief sets the assertion handler to be called when an assertion fails
 * 
 * @param new_handler[in] will be called from now on whenever an assertion fails
 */
void set_assertion_handler(assertion_handler_f new_handler);

/// Replacement for assert() that calls wrapped handler
#define assert(expr)             \
    ((expr)                      \
         ? __ASSERT_VOID_CAST(0) \
         : test_assert(#expr))
#pragma GCC diagnostic pop
#endif