#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include <netinet/in.h> 
#include <sys/time.h>

#include "common.h"
#include "../include/send_udp.h"

// define a pseudo-namespace for private variables only used by the send_udp suite
static struct ns_sendUDP_t {
    int port;
    int sockfd;
} data;

// i heard you like C++, so i put C++ in your C
static struct ns_sendUDP_t *this = &data;

void teardown_send_udp(void)
{
    // close service socket
    close(this->sockfd);
    memset(this, 0, sizeof(*this));
}

//==============================================================================
// sendUDP test suite
//==============================================================================

static void _setup_send_udp(bool ipv4)
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
    if ((this->sockfd = socket(domain, SOCK_DGRAM, 0)) == -1)
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
}

static void test_send_udp(char *addr)
{
    //BSLE_TESTED_JQR_ITEMS(C_NETWORKING, 5, "3.1.12a", "3.1.12d", "3.1.12k", "3.1.12n", "3.1.12o");
    //BSLE_REQUIRED(sendUDP);
    // read a page-size secret from /dev/urandom
    char secret[BUFFER_SIZE] = {0};
    READ_RANDOM(secret);

    // send that secret to our service
    int bytes_sent = send_udp(addr, this->port, secret, sizeof secret);

    // check that the sendUDP function at least reports that it sent the right
    // number of bytes
    CU_ASSERT_TRUE(bytes_sent == 4096);

    // receive the hopefully transmitted secret
    char tx_secret[sizeof secret] = {0};
    if (recv(this->sockfd, tx_secret, sizeof tx_secret, MSG_WAITALL) != sizeof tx_secret)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    // ensure the transmitted secret is the correct one
    CU_ASSERT_TRUE(memcmp(secret, tx_secret, sizeof secret) == 0);
}


//==============================================================================
// IPv4
//==============================================================================
/** implements a service to facilitate testing of the sendUDP() function */
void setup_send_udp(void)
{
    _setup_send_udp(true);
}

void jqr_test_send_udp(void)
{
    setup_send_udp();
    test_send_udp("127.0.0.1");
    teardown_send_udp();
}

//==============================================================================
// IPv6
//==============================================================================

/** implements a service to facilitate testing of the sendUDP() function */
void setup_send_udp6(void)
{
    _setup_send_udp(false);
}

void jqr_test_send_udp6(void)
{
    setup_send_udp6();
    test_send_udp("::1");
    teardown_send_udp();
}
