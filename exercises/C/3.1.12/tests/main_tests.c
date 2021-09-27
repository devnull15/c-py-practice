#include <stdlib.h>
#include <time.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include "common.h"
#include "test_send_udp.h"
#include "test_send_tcp.h"
#include "test_converse_tcp.h"
#include "test_converse_udp.h"
#include "test_broadcast_udp.h"
#include "test_send_unix.h"
#include "test_set_hostname.h"
#include "test_check_functions.h"
#include <sys/time.h>

int main(int argc, char *argv[])
{
    struct timeval tv = {.tv_sec = 0, .tv_usec = 0};
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec / 1000);

    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }
    CU_pSuite jqr_conditionals_test_suite = CU_add_suite("jqr_conditionals_tests", NULL, NULL);

    if (NULL == jqr_conditionals_test_suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(jqr_conditionals_test_suite, "test of ipv4 send_udp()", jqr_test_send_udp)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of ipv6 send_udp()", jqr_test_send_udp6)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of ipv4 send_tcp()", jqr_test_send_tcp)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of ipv6 send_tcp()", jqr_test_send_tcp6)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of ipv4 converse_tcp()", jqr_test_converse_tcp)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of ipv6 converse_tcp()", jqr_test_converse_tcp6)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of ipv4 converse_udp()", jqr_test_converse_udp)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of ipv6 converse_udp()", jqr_test_converse_udp6)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of broadcast_udp()", jqr_test_broadcast_udp)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of send_unix()", jqr_test_send_unix)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of set_hostname()", jqr_test_set_hostname)) ||
        (NULL == CU_add_test(jqr_conditionals_test_suite, "test of check_functions()", jqr_test_check_functions)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_basic_set_mode(CU_BRM_VERBOSE);

    if (argc >= 1)
    {
        CU_set_output_filename(argv[0]);
    }

    CU_automated_enable_junit_xml(CU_TRUE);
    CU_automated_run_tests();
    CU_list_tests_to_file();

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
