#ifndef common_H
#define common_H

#define BUFFER_SIZE 4096

#define PORT_MIN 1024
#define PORT_MAX 65535


//#define RAND_RANGE(lower, upper) ((rand() % (upper - lower + 1)) + lower)

#define SEND(fd, msg) if (send(fd, msg, sizeof msg, 0) == -1)   {   \
    perror("send");                                                 \
    CU_FAIL_FATAL("Networking Error");                                             \
}

#define SENDTO(fd, msg, addr, addrlen) if (sendto(fd, msg, sizeof msg, 0, addr, addrlen) == -1)   { \
    perror("send");                                                                                 \
    CU_FAIL_FATAL("Networking Error");                                                                             \
}

#define RECV(fd, buf) if (recv(sockfd, buf, sizeof buf, 0) == -1) {         \
    perror("recv");                                                         \
    CU_FAIL_FATAL("Networking Error");                                                     \
}

#define TEST_PROTOCOL(fd, buffer, request, response)	 \
{                                                                                \
    if (send(fd, request, strlen(request) + 1, 0) == -1)   {                    \
        perror("send");                                                     \
        CU_FAIL_FATAL("Networking Error (Send)");                                                 \
    }                                                                       \
    memset(buffer, 0, sizeof buffer);                              \
    if (recv(sockfd, buffer, sizeof buffer, 0) < 0) { \
        if (errno == EAGAIN || errno == EWOULDBLOCK) {                      \
            CU_FAIL_FATAL("Networking failure (Recv timeout)");                                \
        } else {                                                            \
            perror("recv");                                                 \
            CU_FAIL_FATAL("Networking Error (Recv)");                                             \
        }                                                                   \
    }                                                                       \
    CU_ASSERT_EQUAL_FATAL(memcmp(response, buffer,                         \
        strlen(response)),0); \
}

#define READ_RANDOM(secret)                                                         \
{                                                                                   \
    FILE *fp = NULL;                                                                \
    if ((fp = fopen("/dev/urandom", "r")) == NULL) {                                \
        perror("fopen");                                                            \
        CU_FAIL_FATAL("Networking Error");                                                         \
    }                                                                               \
                                                                                    \
    int nr_bytes = 0;                                                               \
    int nr_read = 0;                                                                \
    while ((nr_read = fread(secret, 1, (sizeof secret) - nr_bytes, fp)) > 0) {      \
        nr_bytes += nr_read;                                                        \
    }                                                                               \
    assert(nr_bytes == BUFFER_SIZE);                                                       \
    fclose(fp);                                                                     \
}

int rand_range(int min, int max);
int rand_port(void);

#endif
