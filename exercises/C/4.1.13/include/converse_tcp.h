#ifndef CONVERSE_TCP_H
#define CONVERSE_TCP_H

#include "jqr_conditionals.h"

/**
 * @brief starts a server listing on a TCP port and a testing program connects
 * and initiates the following conversations:
 *     Conversation protocol:
 *  +-------------------------------------------+
 *  |Client sends   |Server responds            |
 *  +-------------------------------------------+
 *  |Hi.            |Hello.                     |
 *  |Nice weather?  |Too sunny.                 |
 *  |Ribbit.        |Frogs are slimy.           |
 *  |HOSTNAME       |{hostname}                 |
 *  |SHUTDOWN       |<server closes connection> |
 *  +-------------------------------------------+
 *
 * The server will function by responding to the message and immediately
 * entering recv state to await next action until client termination signal
 * is received or server receives the SHUTDOWN message in which it will
 * terminate the connection
 *
 * Your solution must reuse the same listening port. You are to only set this
 * option if it is currently disabled. That means you must query to determine
 * if the option is enabled/disabled and set it only if it is disabled.
 *
 * YOU MUST USE THE FOLLOWING FUCTIONS FOR THIS EXERCISE:
 *     socket();
 *     gethostname();
 *     getsockopt();
 *     setsockopt();
 *     bind();
 *     accept();
 *     recv();
 *     send();
 *     
 * YOU MUST USE THE FOLLOWING STRUCTURES FOR THIS EXERCISE:
 *     struct sockaddr_in;
 *     struct sockaddr_in6;
 *
 * YOU MUST USE PROPER TYPECASTING USING:
 *     (struct sockaddr*)
 *
 * @param: `port` tcp port number
 *     `ipDomain` IPv4 or IPv6 fqdn or address
 *
 * @return: Not required
*/
void converse_tcp(uint16_t port, int ipDomain);

#endif
