#include <arpa/inet.h>
#include <stdlib.h>
#include <assert.h>
#include <CUnit/CUnit.h>

#include "common.h"
#include "../include/broadcast_udp.h"

#define BUFFER_SIZE 4096
#define TIMEOUT_USEC 10000

//==============================================================================
// broadcastUDP test suite
//==============================================================================

//#define REQS 7, "3.1.12a", "3.1.12d", "3.1.12n", "3.1.12j", "3.1.12o", "3.7.1c", "3.7.2h"

// define a pseudo-namespace for private variables only used by the broadcastUDP suite
static struct ns_broadcastUDP_t {
    int port;
    int sockfd;
    char secret[BUFFER_SIZE];
} data;

static struct ns_broadcastUDP_t *this = &data;

void setup_broadcast_udp()
{
    int yes = 1;
    this->port = rand_port();

    if ((this->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = TIMEOUT_USEC; // generous
    setsockopt(this->sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);
    setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    struct sockaddr_in src_addr = {0};
    src_addr.sin_family = AF_INET;
    src_addr.sin_port = htons(this->port);
    // only receive broadcasts
    src_addr.sin_addr.s_addr = INADDR_BROADCAST;

    if (bind(this->sockfd, (struct sockaddr *) &src_addr, sizeof src_addr) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    READ_RANDOM(this->secret);
}

void test_broadcast_udp(void)
{
    int nr_bytes = broadcast_udp(this->port, this->secret, sizeof this->secret);
    CU_ASSERT_TRUE(nr_bytes == BUFFER_SIZE);

    char buffer[sizeof this->secret] = {0};
    int recv_bytes = recvfrom(this->sockfd, buffer, sizeof buffer, MSG_WAITALL, NULL, NULL);
    CU_ASSERT_TRUE_FATAL(recv_bytes == sizeof buffer);
    CU_ASSERT_TRUE(memcmp(this->secret, buffer, sizeof buffer) == 0);
}

void teardown_broadcast_udp()
{
    close(this->sockfd);
}

void jqr_test_broadcast_udp(void)
{
    setup_broadcast_udp();
    test_broadcast_udp();
    teardown_broadcast_udp();
}
