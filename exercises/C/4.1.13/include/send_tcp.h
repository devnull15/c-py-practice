#ifndef SEND_TCP_H
#define SEND_TCP_H

#include "jqr_conditionals.h"

/**
 * @brief Connects to the supplied IP and sends supplied number of bytes of
 * data from supplied input, closes connection after sending
 *
 * YOU MUST USE THE FOLLOWING FUNCTIONS FOR THIS EXERCISE:
 *     socket();
 *     getaddrinfo();
 *     connect();
 *     send();
 *     
 * @param: `ipStr` IPv4 or IPv6 address
 *     `port` tcp port number
 *     `buffer` address of the data to send
 *     `length` number of bytes to send
 *
 * @return: Return bytes sent or negative value for errors
*/
int send_tcp(char *ipStr, uint16_t port, void *buffer, uint32_t bufSize);

#endif
