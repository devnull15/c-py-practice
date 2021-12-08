#ifndef BROADCAST_UDP_H
#define BROADCAST_UDP_H

#include "jqr_conditionals.h"

/**
 * @brief Broadcasts to the local broadcase address 255.255.255.255 then
 * closes the socket.
 * 
 * YOU MUST USE THE FOLLOWING FUNCTIONS FOR THIS EXERCISE:
 *     socket();
 *     setsockopt();
 *
 * YOU MUST USE THE FOLLOWING STRUCTURES FOR THIS EXERCISE:
 *     struct sockaddr_in;
 *     
 * YOU MUST USE PROPER TYPECASTING USING:
 *     (struct sockaddr*)
 *
 * @param: `port` udp port number
 *     `length` number of bytes to broadcast 
 *     `buffer` address of the data to broadcast
 *
 * @return: Nnumber of bytes sent on success, negative values for errors.
*/
int broadcast_udp(int port, char *buffer, int length);

#endif
