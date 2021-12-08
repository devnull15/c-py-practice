// gives us pthread_timedjoin_np()
#define _GNU_SOURCE

#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/CUnit.h>
#include <sys/time.h>

#include "common.h"
#include "../include/converse_tcp.h"

#define HOST_NAME_MAX 63
#define SLEEP_USEC 100000
#define TIMEOUT_USEC 10000

// define a pseudo-namespace for private variables only used by the converse_tcp
// suite
static struct ns_converseTCP_t
{
    int port;
    char hostname[HOST_NAME_MAX];
    pthread_t thread_id;
} data;

static struct ns_converseTCP_t *this = &data;

static void wrapper(void *data)
{
    converse_tcp(this->port, AF_INET);
}

static void wrapper6(void *data)
{
    converse_tcp(this->port, AF_INET6);
}

void teardown_converse_tcp(void)
{
    if (NULL == &converse_tcp)
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
        CU_FAIL_FATAL("Networking Error");
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
            CU_FAIL_FATAL("Networking Error");
        }
    }

}

//==============================================================================
// converse_tcp test suite
//==============================================================================
static void _setup_converse_tcp(bool ipv4)
{
    if (NULL == &converse_tcp)
    {
        return;
    }
    // seed RNG
    struct timeval tv = {.tv_sec = 0, .tv_usec = 0};
    gettimeofday(&tv, NULL); 
    srand(tv.tv_usec / 1000);

    // get random unprivileged port number
    this->port = rand_port();

    // run the candidate's server in a separate thread so that we can interact
    // with it
    int en;

    void *wrap_func = NULL;
    if (ipv4)
    {
        wrap_func = wrapper;
    }
    else
    {
        wrap_func = wrapper6;
    }

    if ((en = pthread_create(&(this->thread_id), NULL, wrap_func, NULL)) != 0)
    {
        errno = en;
        perror("pthread_create");
        CU_FAIL_FATAL("Networking Error");
    }

    // the server must exist before we send data to it, and we can't ask the
    // candidate to implement synchronization with our test code
    usleep(SLEEP_USEC);

    // get system hostname (needed for conversation)
    if (gethostname(this->hostname, sizeof this->hostname) != 0)
    {
        perror("gethostname");
        CU_FAIL_FATAL("Networking Error");
    }
}

/** converse with the candidate's service */
static void test_converse_tcp(bool ipv4)
{
    int sockfd;
    int domain = 0;
    int yes = 1;
    if (ipv4)
    {
        domain = AF_INET;
    }
    else  //IPv6
    { 
        domain = AF_INET6;
    }

    if ((sockfd = socket(domain, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        CU_FAIL_FATAL("Networking Error");
    }

    // set a timeout for recv, because it shouldn't take more than
    // an instant to get a reply from the server
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = TIMEOUT_USEC; // generous
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    if (ipv4)
    {
        // connect to 127.0,0.1:RHP where the service should be listening
        struct sockaddr_in addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(this->port);
        inet_aton("127.0.0.1", &addr.sin_addr);
        memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

        if (connect(sockfd, (struct sockaddr *) &addr, sizeof addr) == -1)
	{
            // if the connection was refused, it's likely that the candidate didn't
            // properly convert the byte order of the port or otherwise did not set
            // up the server correctly
            if (errno == ECONNREFUSED)
	    {
                close(sockfd);
                CU_FAIL("Connection refused");
            }
	    else
	    {
                // otherwise, something unrecoverable probably happened
                perror("connect");
                CU_FAIL_FATAL("Networking Error");
            }
        }
    }
    else
    {
        // connect to ::1:RHP where the service should be listening
        struct sockaddr_in6 addr = {0};
        addr.sin6_family = AF_INET6;
        addr.sin6_port = htons(this->port);
        addr.sin6_addr = in6addr_any;

        if (connect(sockfd, (struct sockaddr *) &addr, sizeof addr) == -1)
	{
            // if the connection was refused, it's likely that the candidate didn't
            // properly convert the byte order of the port or otherwise did not set
            // up the server correctly
            if (errno == ECONNREFUSED)
	    {
                close(sockfd);
                // candidate fails this test
                CU_FAIL("Connection refused");
            }
	    else
	    {
                // otherwise, something unrecoverable probably happened
                perror("connect");
                CU_FAIL_FATAL("Networking Error");
            }
        }
    }

    // The message order should be arbitrary, but we don't need to randomize
    // it, we'll just do it in an order other than what the docs instruct the
    // candidate to do.
    char buffer[BUFFER_SIZE] = {0};
    TEST_PROTOCOL(sockfd, buffer, "HOSTNAME", this->hostname);
    TEST_PROTOCOL(sockfd, buffer, "Nice weather?", "Too sunny.");
    TEST_PROTOCOL(sockfd, buffer, "Hi.", "Hello.");
    TEST_PROTOCOL(sockfd, buffer, "Ribbit.", "Frogs are slimy.");
    SEND(sockfd, "SHUTDOWN");

    // cleanup
    close(sockfd);
}

//==============================================================================
// IPv4
//==============================================================================
void setup_converse_tcp(void)
{
    _setup_converse_tcp(true);
}

void jqr_test_converse_tcp(void)
{
    setup_converse_tcp();
    test_converse_tcp(true);
    teardown_converse_tcp();
}

//==============================================================================
// IPv6
//==============================================================================
void setup_converse_tcp6(void)
{
    _setup_converse_tcp(false);
}

void jqr_test_converse_tcp6(void)
{
    setup_converse_tcp6();
    test_converse_tcp(false);
    teardown_converse_tcp();
}
