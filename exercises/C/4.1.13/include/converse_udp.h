#ifndef CONVERSE_UPD_H
#define CONVERSE_UPD_H

#include "jqr_conditionals.h"

/**
 * @brief starts a server listing on a UDP port and a testing program connects
 * and initiates the following conversations in binary format:
 *
 *   Conversation protocol:
 *  +------------------------------------------------------------------+
 *  |Client sends           |Server responds                           |
 *  +------------------------------------------------------------------+
 *  |char[4] "PORT"         | (server acknowledges port)               |
 *  |uint16_t port          | uint16_t port                            |
 *  +------------------------------------------------------------------+
 *  |uint32_t 0xCAFEBABE    | uint32_t 0xDFACBABE                      |
 *  +------------------------------------------------------------------+
 *  |uint32_t 0x90          | char[4] "NOOP"                            |
 *  +------------------------------------------------------------------+
 *  |char[4] "ADD2"          | uint32_t sum                             |
 *  |uint16_t num1          |                                          |
 *  |uint16_t num2          | (the sum of the two numbers)             |
 *  +------------------------------------------------------------------+
 *  |uint32_t 0x1           | uint32_t {packed IPv4 address}           |
 *  |char[] {hostname}      |                                          |
 *  |                       | when packing the IP the most significant |
 *  |                       | byte should be the left-most octet of the|
 *  |                       | IP address; send the value in network    |
 *  |                       | byte order (big endian)                  |
 *  |                       |                                          |
 *  |                       | for example: 192.168.0.1 becomes:        |
 *  |                       | 0xc0a80001                               |
 *  |                       |                                          |
 *  |                       | HINT: the hostname this challenge asks   |
 *  |                       | you to resolve is well known.            |
 *  +------------------------------------------------------------------+
 *  +------------------------------------------------------------------+
 *  |uint32_t 0xFFFFFFFF    | 0xFFFFFFFF (Conversation ends)           |
 *  +------------------------------------------------------------------+
 *
 * All integer values must be sent to the client in big-endian byte order.
 *
 * The client will send the callback port as its first message. The server must
 * take that port number and use it for all responses to the client until
 * disconnect. The port will be in network byte order, and is the one exception
 * where the server responds with 2 bytes instead of 4.
 *
 * The protocol consists of a 32-bit opcode followed by optional (or no)
 * data. Above, this opcode is sometimes described as a string of 4
 * characters. HINT: use the fact that that this can also be read into a
 * 32-bit integer and it will make solving the problem easier.
 *
 * This function will initiate the communication by sending a message and
 * immediately entering recv state to await next action, until client
 * termination signal is received.
 *
 * YOU MUST USE THE FOLLOWING FUNCTIONS FOR THIS EXERCISE:
 *     socket();
 *     bind();
 *     sendto();
 *     recvfrom();
 *     
 * YOU MUST USE THE FOLLOWING STRUCTURES FOR THIS EXERCISE:
 *     struct sockaddr_in;
 *     struct sockaddr_in6;
 *
 * YOU MUST USE PROPER TYPECASTING USING:
 *     (struct sockaddr *)
 *
 * @param: `port` udp port number
 *     `ipDomain` IPv4 or IPv6 fqdn or address
 *
 * @return: Not required
*/
void converse_udp(uint16_t port, int ipDomain);

#endif
