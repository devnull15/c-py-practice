#include "../include/helper.h"

//this is copied code because I'm not fucking with CMake
void print_sockaddr_tcp(struct sockaddr_storage *in) {

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
 *     `bufSize` number of bytes to send
 *
 * @return: Return bytes sent or negative value for errors
*/
int send_tcp(char *ipStr, uint16_t port, void *buffer, uint32_t bufSize) {

  //debug
  fprintf(stderr, "\n---------------------------------------\n");
  fprintf(stderr, "send_tcp sending buffer (size:%i) to %s:%i\n",bufSize,ipStr,port);
  //debug

  char p[6] = {0};
  snprintf(p, sizeof(p), "%i", port);
  int err;
  
  //**getaddrinfo stuff
  // first hints
  struct addrinfo hints, *res;
  memset (&hints, 0, sizeof (hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  // getaddrinfo lookup
  if((err = getaddrinfo(ipStr,p,&hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo failed = %i\n", err);
    perror("* getaddrinfo error");
    return err;
  }
  //**getaddrinfo succes
  
  //debug
  struct sockaddr_storage *target;
  target = (struct sockaddr_storage *) res->ai_addr;
  print_sockaddr_tcp(target);
  //debug

  
  // res is a ll; but I'm assuming I'm getting an IP address so there should only be one entry in the list

  
  //**Make Socket
  int sockfd;
  if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0 ) {
    fprintf(stderr, "sockfd failed = %i\n", sockfd);
    perror("* socket error");
    return sockfd;
  }
  //**Socket success

  //**Connect
  if((err = connect(sockfd, res->ai_addr,res->ai_addrlen)) < 0) {
    fprintf(stderr, "connect failed = %i\n", err);
    perror("* connect error");
    return err;
  }
  //**Connect done
  
  //**send
  ssize_t bytes_sent;
  
  if((bytes_sent = send(sockfd, buffer, (size_t)bufSize, 0)) < 0) {
    fprintf(stderr, "send failed = %li\n", bytes_sent);
    perror("* send error");
    return bytes_sent;
  }
  //**send success

  //**cleanup
  freeaddrinfo(res);
  //**cleanup done

  //**debug
  fprintf(stderr, "Done!\n");
  //**debug
  
  return bytes_sent;


  
  return 0;
}
