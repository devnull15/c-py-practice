#include <sys/socket.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <sys/un.h>
#include <CUnit/CUnit.h>

#include "common.h"
#include "../include/send_unix.h"

#define BACKLOG 10


// define a pseudo-namespace for private variables only used by the send_unix suite
static struct ns_sendUnix_t
{
	char path[PATH_MAX];
    int sockfd;
} data;

// i heard you like C++, so i put C++ in your C
static struct ns_sendUnix_t *this = &data;

/** implement a mock service to test the candidate's code */
void setup_send_unix()
{
    int yes = 1;
	snprintf(this->path, sizeof this->path - 1, "/tmp/bsle_test_sock.%ld",  time(NULL));

	if ((this->sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	struct sockaddr_un src_addr = {0};
	src_addr.sun_family = AF_UNIX;
	strncpy(src_addr.sun_path, this->path, sizeof src_addr.sun_path - 1);

	if ((bind(this->sockfd, (struct sockaddr *) &src_addr, sizeof(src_addr))) == -1)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(this->sockfd, BACKLOG) == -1)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
}

void test_send_unix(void)
{
    // read a page-size secret from /dev/urandom
    char secret[BUFFER_SIZE] = {0};
    READ_RANDOM(secret);

    // send that secret to our listening service
    int bytes_sent = send_unix(this->path, secret, BUFFER_SIZE);

    // check that the send_unix function at least reports that it sent the right
    // number of bytes
    CU_ASSERT_TRUE(bytes_sent == BUFFER_SIZE);

    // accept the connection that should have occurred as a result of calling
    // the candidate's send_unix() function
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

void teardown_send_unix()
{
	close(this->sockfd);
	unlink(this->path);
}

void jqr_test_send_unix(void)
{
    setup_send_unix();
    test_send_unix();
    teardown_send_unix();
}
