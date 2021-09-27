#include "wrap_assert.h"
#include <stdio.h>
#include <stdlib.h>

__attribute__((noreturn)) static void default_assertion_handler(char *expression)
{
    printf("Assertion: %s failed\n", expression);
    abort();
}
static assertion_handler_f _test_assert = default_assertion_handler;

void set_assertion_handler(assertion_handler_f new_handler)
{
    printf("New assertion handler set...\n");
    _test_assert = new_handler;
}

void test_assert(char *expression)
{
    printf("Inside test_assert\n");
    _test_assert(expression);
}
