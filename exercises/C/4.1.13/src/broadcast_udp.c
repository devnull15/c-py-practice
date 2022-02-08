#include "../include/helper.h"

//this is copied code because I'm not fucking with CMake
void print_sockaddr_budp(struct sockaddr_storage *in) {

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
int broadcast_udp(int port, char *buffer, int length) {

  //debug
  fprintf(stderr, "\n---------------------------------------\n");
  fprintf(stderr, " broadcast_udp sending buffer size:%i to %s:%i\n",length,buffer,port);
  //debug

  char p[6];
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
  if((err = getaddrinfo("255.255.255.255",p,&hints, &res)) != 0) {
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
  int optval = 1;
  if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval))) {
    perror("* setsockopt error");
    return -1;
  }
  //**Socket success

  //**sendto
  struct sockaddr_storage *target;
  ssize_t bytes_sent;
  socklen_t target_len;
  target = (struct sockaddr_storage *) res->ai_addr;
  target_len = res->ai_addrlen;
  
  //debug
  print_sockaddr_budp (target);
  //debug
  
  if((bytes_sent = sendto(sockfd, buffer, length, 0, (struct sockaddr *) target, target_len)) < 0) {
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
