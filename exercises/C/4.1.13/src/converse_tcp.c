#include "../include/helper.h"

#define HOST_NAME_MAX 64 //POSIX.1

//this is copied code because I'm not fucking with CMake
void print_sockaddr_tcp_serv(struct sockaddr_storage *in) {

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

void _respond_tcp(char *rec, char *sen) {
  
  //debug
  fprintf(stderr, "received: %s\n", rec);
  //debug
  
  int len = HOST_NAME_MAX;
  if(strcmp(rec,"Hi.")==0) { strcpy(sen,"Hello."); }
  else if(strcmp(rec,"Nice weather?")==0) { strcpy(sen,"Too sunny."); }
  else if(strcmp(rec,"Ribbit.")==0) { strcpy(sen,"Frogs are slimy."); }
  else if(strcmp(rec,"HOSTNAME")==0) {
    if(gethostname(sen,len) < 0) {
      fprintf(stderr,"* gethostname error\n");
      perror("gethostname");
    }
  }
  else if(strcmp(rec,"SHUTDOWN")==0) { strcpy(sen,"SHUTDOWN"); }
  else { strcpy(sen,"Invalid command."); }

  return;
}


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
*  @return: Not required
*/
void converse_tcp(uint16_t port, int ipDomain) {

  //debug
  fprintf(stderr, "\n---------------------------------------\n");
  fprintf(stderr, "converse_tcp listening on %i:%hu\n",ipDomain,port);
  //debug

  //**getaddrinfo stuff
  // vars
  char p[5];
  sprintf(p, "%i", port);
  int err;

  // hints
  struct addrinfo hints, *res;
  memset (&hints, 0, sizeof (hints));
  hints.ai_family = ipDomain;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // getaddrinfo lookup
  if((err = getaddrinfo(NULL,p,&hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo failed = %i\n", err);
    perror("* getaddrinfo error");
    return;
  }
  //**getaddrinfo succes
  
  //debug
  struct sockaddr_storage *target;
  target = (struct sockaddr_storage *) res->ai_addr;
  print_sockaddr_tcp_serv(target);
  //debug

  
  //**Make Socket
  int sockfd;
  if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0 ) {
    fprintf(stderr, "sockfd failed = %i\n", sockfd);
    perror("* socket error");
    return;
  }
  //**Socket success

  //**get/set sockopt
  int optval = 0;
  socklen_t optlen = 0;
  getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen);
  if(optval!=1) {
    fprintf(stderr, "SO_REUSEADDR not set, fixing it...\n");
    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  }  
  //**get/set sockopt done
  
  //**bind
  if (bind(sockfd, res->ai_addr, res->ai_addrlen)<0) {
    fprintf(stderr, "* bind error\n");
    perror("bind"); 
    return;
  }
  //**bind done
  
  //**cleanup
  freeaddrinfo(res);
  //**cleanup done

  //**listen
  if((err = listen(sockfd,1)) < 0) {
    fprintf(stderr, "* listen error\n");
    perror("listen");
  }
  //**listen done
  
  
  //**send/recv loop
  char rec[HOST_NAME_MAX] = {0};
  char sen[HOST_NAME_MAX] = {0};
  struct sockaddr_storage client;
  socklen_t client_len = sizeof(client);
  int cnct;
  if((cnct = accept(sockfd, (struct sockaddr *)&client, &client_len)) < 0) {
      fprintf(stderr, "* accept error\n");
      perror("accept");
    }
  else {
    fprintf(stderr, "connection from:\n");
    print_sockaddr_tcp_serv(&client);
  }
  while((strcmp(rec,"SHUTDOWN"))!=0) {
    if(recv(cnct, rec, HOST_NAME_MAX,0) < 0) {
      fprintf(stderr, "* recv error\n");
      perror("recv");
      return;
    }
    
    _respond_tcp(rec,sen);
    
    //debug
    fprintf(stderr, "sending: %s\n", sen);
    //debug
    
    if(send(cnct, sen, HOST_NAME_MAX,0) < 0) {
      fprintf(stderr, "* send error\n");
      perror("send");
    }
  }
  //**send/recs loop done

  //**close
  close(cnct);
  close(sockfd);
  //**close done

  //**debug
  fprintf(stderr, "Done!\n");
  //**debug
  
  
  return;
}
