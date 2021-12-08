/*
 * Checks to ensure that functions were used in the program.
 * Functions that must be used for this jqr item are:
 * 
 * accept
 * bind
 * connect
 * getaddrinfo
 * gethostname
 * send
 * sendto
 * sethostname
 * setsockopt
 * socket
 */

#include <stdlib.h>
#include <CUnit/CUnit.h>

void jqr_test_check_functions(void)
{
    if (system("bash -c \"grep -q 'accept@@' <(nm build/${BUILD_MODULE}_exe)\""))
    {
        printf("Failed to use accept() function ");
        CU_FAIL_FATAL("Failed to use accept() function");
    }
    else
    {
        printf("Found accept() function ");
        CU_PASS("Found accept() function");
    }

    if (system("bash -c \"grep -q 'bind@@' <(nm build/${BUILD_MODULE}_exe)\""))
    {
        printf("Failed to use bind() function ");
        CU_FAIL_FATAL("Failed to use bind() function");
    }
    else
    {
        printf("Found bind() function ");
        CU_PASS("Found bind() function");
    }

    if (system("bash -c \"grep -q 'connect@@' <(nm build/${BUILD_MODULE}_exe)\""))
    {
        printf("Failed to use connect() function ");
        CU_FAIL_FATAL("Failed to use connext() function");
    }
    else
    {
        printf("Found connect() function ");
        CU_PASS("Found connnect() function");
    }

    if (system("bash -c \"grep -q 'getaddrinfo@@' <(nm build/${BUILD_MODULE}_exe)\""))
    {
        printf("Failed to use getaddrinfo() function ");
        CU_FAIL_FATAL("Failed to use getaddrinfo() function");
    }
    else
    {
        printf("Found getaddrinfo() function ");
        CU_PASS("Found getaddrinfo() function");
    }

    if (system("bash -c \"grep -q 'gethostname@@' <(nm build/${BUILD_MODULE}_exe)\""))
    {
        printf("Failed to use gethostname() function ");
        CU_FAIL_FATAL("Failed to use gethostname() function");
    }
    else
    {
        printf("Found gethostname() function ");
        CU_PASS("Found gethostname() function");
    }

    if (system("bash -c \"grep -q 'send@@' <(nm build/${BUILD_MODULE}_exe)\""))
    {
        printf("Failed to use send() function ");
        CU_FAIL_FATAL("Failed to use send() function");
    }
    else
    {
        printf("Found send() function ");
        CU_PASS("Found send() function");
    }

    if (system("bash -c \"grep -q 'sendto@@' <(nm build/${BUILD_MODULE}_exe)\""))
    {
        printf("Failed to use sendto() function ");
        CU_FAIL_FATAL("Failed to use sendto() function");
    }
    else
    {
        printf("Found sendto() function ");
        CU_PASS("Found sendto() function");
    }

    if (system("bash -c \"grep -q 'sethostname@@' <(nm build/${BUILD_MODULE}_exe)\""))
    {
        printf("Failed to use sethostname() function ");
        CU_FAIL_FATAL("Failed to use sethostname() function");
    }
    else
    {
        printf("Found sethostname() function ");
        CU_PASS("Found sethostname() function");
    }

    if (system("bash -c \"grep -q 'setsockopt@@' <(nm build/${BUILD_MODULE}_exe)\""))
    {
        printf("Failed to use setsockopt() function  ");
        CU_FAIL_FATAL("Failed to use setsockopt() function");
    }
    else
    {
        printf("Found setsockopt() function ");
        CU_PASS("Found setsockopt() function");
    }

    if (system("bash -c \"grep -q 'socket@@' <(nm build/${BUILD_MODULE}_exe)\""))
    {
        printf("Failed to use socket() function  ");
        CU_FAIL_FATAL("Failed to use socket() function");
    }
    else
    {
        printf("Found socket() function ");
        CU_PASS("Found socket() function");
    }

}
