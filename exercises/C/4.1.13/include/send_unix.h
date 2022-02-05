#ifndef SEND_UNIX_H
#define SEND_UNIX_H

#include "jqr_conditionals.h"

/*
    send_unix()

    Connects to a `SOCK_STREAM` Unix socket specified by `path`, sends `len`
    bytes of contents from `buffer`, closes the socket.

    Returns number of bytes sent.
*/
/**
 * @brief Connects to a `SOCK_STREAM` Unix socket specified and sends
 * specified number of bytes from supplied buffer, then closes the socket
 *
 * YOU MUST USE THE FOLLOWING FUNCTIONS FOR THIS EXERCISE:
 *     socket();
 *     connect();
 *     send();
 *
 *  YOU MUST USE THE FOLLOWING STRUCTURES FOR THIS EXERCISE:
 *     struct sockaddr_un;
 *
 * YOU MUST USE PROPER TYPECASTING USING:
 *     (struct sockaddr*)
 *
 * @param: `ipStr`
 *     `path` path to unix socket
 *     `buffer` address of the data to send
 *     `len` number of bytes from buffer to send
 *
 * @return: Return bytes sent
*/

int send_unix(char *path, char *buffer, int len);

#endif
