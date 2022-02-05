#ifndef SEND_UDP_H
#define SEND_UDP_H

#include "jqr_conditionals.h"

/**
 * @brief Sends to the supplied IP, the supplied number of bytes of
 * data from supplied input
 *
 * YOU MUST USE THE FOLLOWING FUNCTIONS FOR THIS EXERCISE:
 *     socket();
 *     getaddrinfo();
 *     sendto();
 *
 * @param: `ipStr` IPv4 or IPv6 address
 *     `port` UDP port number
 *     `buffer` address of the data to send
 *     `length` number of bytes to send
 *
 * @return: Return bytes sent or negative value for errors
 *
 * @todo: discuss this; it doesn't test anything that the sendTCP test isn't
 * already testing; it should test that the user knows that he has to manually
 * resequence the application layer PDU since it is sent via UDP and
 * potentially deal with missing packets
*/
int send_udp(char *ipStr, uint16_t port, void *buffer, uint32_t bufSize);

#endif
