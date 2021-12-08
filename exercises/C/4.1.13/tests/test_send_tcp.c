#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <poll.h>
#include <stdbool.h>
#include <CUnit/CUnit.h>
#include <netinet/in.h>
#include <sys/time.h>

#include "common.h"
#include "../include/send_tcp.h"

#define BACKLOG 10
#define ACCEPT_TIMEOUT_MS 1000

// define a pseudo-namespace for private variables only used by the sendTCP suite
static struct ns_sendTCP_t {
    int port;
    int sockfd;
} data;

// i heard you like C++, so i put C++ in your C
static struct ns_sendTCP_t *this = &data;

void teardown_send_tcp(void)
{
    // close service socket
    close(this->sockfd);
    memset(this, 0, sizeof(*this));
}

//==============================================================================
// sendTCP test suite
//==============================================================================

static void _setup_send_tcp(bool ipv4)
{
    // seed RNG
    struct timeval tv = {.tv_sec = 0, .tv_usec = 0};
    gettimeofday(&tv, NULL); 
    srand(tv.tv_usec / 1000);

    // get random unprivileged port number
    this->port = rand_port();
    int domain = 0;
    int yes = 1;

    // create mock endpoint
    if (ipv4)
    {
        domain = AF_INET;
    }
    else //IPv6
    {
        domain = AF_INET6;
    }

    // create mock endpoint
    if ((this->sockfd = socket(domain, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    // bind to random port
    void *addr = NULL;
    socklen_t addrlen = 0;

    if (ipv4)
    {
        struct sockaddr_in _addr = {0};
        _addr.sin_family = AF_INET;
        _addr.sin_port = htons(this->port);

        addr = &_addr;
        addrlen = sizeof(_addr);
    }
    else //IPv6
    {
        struct sockaddr_in6 _addr6 = {0};
        _addr6.sin6_family = AF_INET6;
        _addr6.sin6_port = htons(this->port);
    
        addr = &_addr6;
        addrlen = sizeof(_addr6);
    }

    if (bind(this->sockfd, (struct sockaddr *) addr, addrlen) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // listen for connections from the candidate's sendTCP() function
    if (listen(this->sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

static void test_send_tcp(char *addr)
{
    // read a page-size secret from /dev/urandom
    char secret[BUFFER_SIZE] = {0};
    READ_RANDOM(secret);

    // send that secret to our listening service
    int bytes_sent = send_tcp(addr, this->port, secret, sizeof secret);

    // check that the send_tcp function at least reports that it sent the right
    // number of bytes
    CU_ASSERT_TRUE(bytes_sent == 4096);
    struct pollfd poller = {
        .fd = this->sockfd,
        .events = POLLIN | POLLERR
    };
    if (poll(&poller, 1, ACCEPT_TIMEOUT_MS) <= 0)
    {
        CU_FAIL_FATAL("Timed out...\n");
    }
    // accept the connection that should have occurred as a result of calling
    // the candidate's send_tcp() function
    int connfd = accept(this->sockfd, NULL, NULL);
    if (connfd == -1)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // receive the hopefully transmitted secret
    char tx_secret[sizeof secret] = {0};
    if (recv(connfd, tx_secret, sizeof tx_secret, MSG_WAITALL) != sizeof tx_secret)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    // ensure the transmitted secret is the correct one
    CU_ASSERT_TRUE(memcmp(secret, tx_secret, sizeof secret) == 0);

    // close the mock service's connection
    close(connfd);
}

//==============================================================================
// IPv4
//==============================================================================
/** implements a service to facilitate testing of the send_tcp() function */
void setup_send_tcp(void)
{
    _setup_send_tcp(true);
}

void jqr_test_send_tcp(void)
{
    setup_send_tcp();
    test_send_tcp("127.0.0.1");
    teardown_send_tcp();
}

//==============================================================================
// IPv6
//==============================================================================

/** implements a service to facilitate testing of the send_tcp() function */
void setup_send_tcp6(void)
{
    _setup_send_tcp(false);
}

void jqr_test_send_tcp6(void)
{
    setup_send_tcp6();
    test_send_tcp("::1");
    teardown_send_tcp();
}
