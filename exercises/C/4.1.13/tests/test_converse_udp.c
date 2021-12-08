// gives us pthread_timedjoin_np()
#define _GNU_SOURCE

#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <assert.h>
#include <printf.h>
#include <stdbool.h>
#include <CUnit/CUnit.h>
#include <sys/time.h>

#include "common.h"
#include "../include/converse_udp.h"

#define SLEEP_USEC 100000
#define TIMEOUT_USEC 10000
#define HOST_NAME_MAX 63

typedef enum parm_type_t
{
    NULL_TERMINATED = 0,
    SIZE_INCLUDED
} parm_type_t;


int printf_buffer_print (FILE *stream, const struct printf_info *info,
		    const void *const *args)
{
    const uint8_t *buffer = *((const uint8_t **)args[0]);
    size_t buffer_size = *((size_t *)(args[1]));

    for (size_t i = 0; i < buffer_size; i++)
    {
        fprintf(stream, "%02x", buffer[i]);
    }
    return 2 * buffer_size;
}

int printf_buffer_info(const struct printf_info *info, size_t n, int *argtypes, int *argsizes)
{
    argtypes[0] = PA_POINTER; argsizes[0] = sizeof(void *);
    if (n >= 2)
    {
        argtypes[1] = PA_INT | PA_FLAG_LONG;     argsizes[1] = sizeof(size_t);
    }
    return 2;
}
__attribute__((constructor)) void register_buffer_with_printf()
{
    register_printf_specifier('B', printf_buffer_print, printf_buffer_info);
} 

typedef struct test_data_t
{    
    parm_type_t req_type;
    uint32_t req_num;
    const uint8_t *req_bytes;
    size_t req_size;
    parm_type_t resp_type;
    uint32_t resp_num;
    const uint8_t *resp_bytes;
    size_t resp_size;

} test_data_t;

bool step_test(int fd, struct sockaddr *addr, size_t addrlen, test_data_t *data)
{
    bool result = false;
    uint8_t buffer[BUFFER_SIZE];
    const uint8_t *sendptr = buffer;
    size_t send_size = sizeof(uint32_t);

    if (NULL == data->req_bytes)
    {
        * (uint32_t *) buffer = htonl(data->req_num);
    }
    else if (NULL_TERMINATED == data->req_type)
    {
        assert(4 == strlen((char *) data->req_bytes));
        sendptr = data->req_bytes;
    }
    else if (SIZE_INCLUDED == data->req_type)
    {
        sendptr = data->req_bytes;
        send_size = data->req_size;
    }
    else 
    {
        assert(0);
    }
    if (-1 == sendto(fd, sendptr, send_size, 0, addr, addrlen))
    {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    memset(buffer, 0, sizeof buffer);
    uint32_t expected;
    const uint8_t *expected_ptr = (uint8_t *) &expected;
    size_t expected_size = sizeof(uint32_t);
    if (NULL == data->resp_bytes)
    {
        expected = htonl(data->resp_num);
    }
    else if (NULL_TERMINATED == data->resp_type)
    {
        assert(4 == strlen((char *) data->resp_bytes));
        expected_ptr = data->resp_bytes;
    }
    else if (SIZE_INCLUDED == data->resp_type)
    {
        expected_ptr = data->resp_bytes;
        expected_size = data->resp_size;
    }
    ssize_t actual_size = recv(fd, buffer, sizeof(buffer), 0);
    if (-1 == actual_size)
    {
        if (EAGAIN == errno || EWOULDBLOCK == errno)
        {
            CU_FAIL("Timed out waiting for response");
            goto end;
        }
        perror("recv");
        exit(EXIT_FAILURE);
    }
    if (actual_size == expected_size && 0 == memcmp(expected_ptr, buffer, expected_size))
    {
        result = true;
    }
    else
    {
        char *errbuf = NULL;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
        printf("Expected buffer {%B} did not match actual {%B}\n", expected_ptr, expected_size, buffer, actual_size);
#pragma GCC diagnostic pop
        CU_FAIL("Expected buffer did not match actual (see output above for more detail)");
    }
    
end:
    return result;
}


// define a pseudo-namespace for private variables only used by the converse_udp
// suite
static struct ns_converseUDP_t
{
    int dst_port;
    int src_port;
    char hostname[HOST_NAME_MAX];
    pthread_t thread_id;
} data;

static struct ns_converseUDP_t *this = &data;

static void wrapper(void *data)
{
    converse_udp(this->dst_port, AF_INET);
}

static void wrapper6(void *data)
{
    converse_udp(this->dst_port, AF_INET6);
}

void teardown_converse_udp(void)
{
    if (NULL == converse_udp)
    {
        return;
    }
    // tear down candidate server thread
    int en = 0;

    // get the system clock's elapsed time
    struct timespec ts = {0};
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    // approx one second from now
    ts.tv_sec += 1;

    // give the server a one-second grace period before deciding that
    // it must not be shutting down properly; after that, the candidate
    // fails this test
    if ((en = pthread_timedjoin_np(this->thread_id, NULL, &ts)) != 0)
    {
        if (en == ETIMEDOUT)
	{
            // the candidate fails, since his server didn't shut down
            CU_FAIL("Server didn't shut down");
        }
	else
	{
            errno = en;
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }
}

//==============================================================================
// converse_udp test suite
//==============================================================================
void _setup_converse_udp(bool ipv4)
{
    if (NULL == &converse_udp)
    {
        return;
    }
    // seed RNG
    struct timeval tv = {.tv_sec = 0, .tv_usec = 0};
    gettimeofday(&tv, NULL); 
    srand(tv.tv_usec / 1000);

    // get random unprivileged port number
    this->dst_port = rand_port();
    this->src_port = this->dst_port + 1;

    // run the candidate's server in a separate thread so that we can interact
    // with it
    int en;

    void *wrap_func = NULL;
    if (ipv4)
    {
        wrap_func = wrapper;
    }
    else {
        wrap_func = wrapper6;
    }

    if ((en = pthread_create(&(this->thread_id), NULL, wrap_func, NULL)) != 0)
    {
        errno = en;
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // the server must exist before we send data to it, and we can't ask the
    // candidate to implement synchronization with our test code
    usleep(SLEEP_USEC);

    // get system hostname (needed for conversation)
    if (gethostname(this->hostname, sizeof this->hostname) != 0)
    {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }
}

/** converse with the candidate's service */
static void test_converse_udp(bool ipv4)
{
    int sockfd;
    int domain = 0;
    int yes = 1;
    if (ipv4)
    {
        domain = AF_INET;
    }
    else
    {
        domain = AF_INET6; 
    }

    if ((sockfd = socket(domain, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // set a timeout for recv, because it shouldn't take more than
    // an instant to get a reply from the server
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = TIMEOUT_USEC; // generous

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv) == -1 ||
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    void *src_addr =NULL;
    socklen_t src_addr_len = 0;
    void *dst_addr = NULL;
    socklen_t dst_addr_len = 0;

    if (ipv4)
    {
        struct sockaddr_in _src_addr = {0};
        _src_addr.sin_family = AF_INET;
        _src_addr.sin_port = htons(this->src_port);
        inet_aton("127.0.0.1", &_src_addr.sin_addr);
        src_addr = &_src_addr;
        src_addr_len = sizeof(_src_addr);

        struct sockaddr_in _dst_addr = {0};
        _dst_addr.sin_family = AF_INET;
        _dst_addr.sin_port = htons(this->dst_port);
        inet_aton("127.0.0.1", &_dst_addr.sin_addr);

        dst_addr = &_dst_addr;
        dst_addr_len = sizeof(_dst_addr);

    }
    else
    {
        struct sockaddr_in6 _src_addr = {0};
        _src_addr.sin6_family = AF_INET6;
        _src_addr.sin6_port = htons(this->src_port);
        _src_addr.sin6_addr = in6addr_any;
        src_addr = &_src_addr;
        src_addr_len = sizeof(_src_addr);

        struct sockaddr_in6 _dst_addr = {0};
        _dst_addr.sin6_family = AF_INET6;
        _dst_addr.sin6_port = htons(this->dst_port);
        _dst_addr.sin6_addr = in6addr_any;

        dst_addr = &_dst_addr;
        dst_addr_len = sizeof(_dst_addr);

    }
    // both ends need to bind to the port so that bidirectional communication
    // can occur
    if (bind(sockfd, (struct sockaddr*) src_addr, src_addr_len) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // The message order should be arbitrary, but we don't need to randomize
    // it, we'll just do it in an order other than what the docs instruct the
    // candidate to do.

    // we need to tell the server what port to send data back on
    char port_msg[6] = "PORT"; //NOLINT(readability-magic-numbers)
    ((uint16_t *) port_msg)[2] = htons(this->src_port);

    // used to compare the server's expected response for the port command
    char port_resp[2] = {0};
    port_resp[0] = port_msg[4]; //NOLINT(readability-magic-numbers)
    port_resp[1] = port_msg[5]; //NOLINT(readability-magic-numbers)


    CU_ASSERT_TRUE_FATAL(step_test(sockfd, dst_addr, dst_addr_len, &(test_data_t)
    {
        .req_type = SIZE_INCLUDED,
        .req_bytes = (uint8_t *) port_msg,
        .req_size = 6,
        .resp_type = SIZE_INCLUDED,
        .resp_bytes = (uint8_t *) port_resp,
        .resp_size = 2,
    }));

    CU_ASSERT_TRUE_FATAL(step_test(sockfd, dst_addr, dst_addr_len, &(test_data_t)
    {
        .req_num = 0xcafebabe,
        .resp_num = 0xdfacbabe
    }));

    CU_ASSERT_TRUE_FATAL(step_test(sockfd, dst_addr, dst_addr_len, &(test_data_t)
    {
        .req_num = 0x90,
        .resp_bytes = (uint8_t *) "NOOP" 
    }));

    CU_ASSERT_TRUE_FATAL(step_test(sockfd, dst_addr, dst_addr_len, &(test_data_t)
    {
        .req_type = SIZE_INCLUDED,
        .req_bytes = (uint8_t *) "ADD2\xff\xff\xff\xff", 
        .req_size = 8,
        .resp_num = 0x0001fffe
    }));

    CU_ASSERT_TRUE_FATAL(step_test(sockfd, dst_addr, dst_addr_len, &(test_data_t)
    {
        .req_type = SIZE_INCLUDED,
        .req_bytes = (uint8_t *) "\x00\x00\x00\x01localhost",
        .req_size = sizeof("\x00\x00\x00\x01localhost") - 1,
        .resp_num = 0x7f000001
    }));
    //TEST_PROTOCOL_UDP(sockfd, buffer, "\x00\x00\x00\x01localhost", "\x01\x00\x00\x7f", dst_addr, dst_addr_len);
    // terminate server
    SENDTO(sockfd, "\xFF\xFF\xFF\xFF", dst_addr, dst_addr_len);

    // cleanup
    close(sockfd);

    // scan code to ensure proper function usage
    //int testfunc = system("bash -c 'grep -q recvfrom <(nm build/*_exe)'");
    //CU_ASSERT_EQUAL(0, system("bash -c 'grep -q recvfrom <(nm build/*_exe)'"));
}

//==============================================================================
// IPv4
//==============================================================================
void setup_converse_udp(void)
{
    _setup_converse_udp(true);
}

void jqr_test_converse_udp(void)
{
    setup_converse_udp();
    test_converse_udp(true);
    teardown_converse_udp();
    CU_ASSERT_FALSE(system("bash -c 'grep -q recvfrom <(nm build/*_exe)'"));
}

//==============================================================================
// IPv6
//==============================================================================
void setup_converse_udp6(void)
{
    _setup_converse_udp(false);
}

void jqr_test_converse_udp6(void)
{
    setup_converse_udp6();
    test_converse_udp(false);
    teardown_converse_udp();
}
