#include "../include/helper.h"

//this is copied code because I'm not fucking with CMake
void print_sockaddr_udp(struct sockaddr_storage *in) {

  char addr[INET6_ADDRSTRLEN];
  uint16_t port;

  switch(in->ss_family) {

  case AF_INET:
    inet_ntop(in->ss_family, &((struct sockaddr_in *)in)->sin_addr, addr, sizeof(addr));
    port = ntohs(((struct sockaddr_in *)in)->sin_port);
    break;
      
  case AF_INET6:
    inet_ntop(in->ss_family, &((struct sockaddr_in6 *)in)->sin6_addr, addr, sizeof(addr));
    port = ntohs(((struct sockaddr_in6 *)in)->sin6_port);
    break;
  }

  
  fprintf(stderr,"saddr = family:%d (2=AF_INET, 10=AF_INET6)\n\t%s:%d\n",
	  in->ss_family, addr, port);

  
  return;
}


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
int send_udp(char *ipStr, uint16_t port, void *buffer, uint32_t bufSize) {
  
  //debug
  fprintf(stderr, "\n---------------------------------------\n");
  fprintf(stderr, "send_udp sending buffer size:%i to %s:%i\n",bufSize,ipStr,port);
  //debug

  char p[6] = {0};
  snprintf(p, sizeof(p), "%i", port);
  int err;
  
  //**getaddrinfo stuff
  // first hints
  struct addrinfo hints, *res;
  memset (&hints, 0, sizeof (hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = 0;
  // getaddrinfo lookup
  if((err = getaddrinfo(ipStr,p,&hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo failed = %i\n", err);
    return err;
  }
  //**getaddrinfo succes

  
  // res is a ll; but I'm assuming I'm getting an IP address so there should only be one entry in the list

  
  //**Make Socket
  int sockfd;
  if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0 ) {
    fprintf(stderr, "sockfd failed = %i\n", sockfd);
    return sockfd;
  }
  //**Socket success

  //**sendto
  struct sockaddr_storage *target;
  ssize_t bytes_sent;
  socklen_t target_len;
  target = (struct sockaddr_storage *) res->ai_addr;
  target_len = res->ai_addrlen;
  
  //debug
  print_sockaddr_udp (target);
  //debug
  
  if((bytes_sent = sendto(sockfd, buffer, (size_t)bufSize, 0, (struct sockaddr *) target, target_len)) < 0) {
    fprintf(stderr, "sendto failed = %li\n", bytes_sent);
    perror("* sendto error");
    return bytes_sent;
  }
  //**sendto success

  //**cleanup
  freeaddrinfo(res);
  //**cleanup done

  //**debug
  fprintf(stderr, "Done!\n");
  //**debug
  
  return bytes_sent;
}
