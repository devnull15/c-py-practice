#include "../include/helper.h"
#define HOST_NAME_MAX 64
#define OPCODE_LEN 4
#define SERV_SEN_LEN 4


/* This code is disgusting please forgive me. */


//this is copied code because I'm not fucking with CMake
void print_sockaddr_udp_serv(struct sockaddr_storage *in) {
    
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

void _change_port(struct sockaddr_storage *in, uint16_t port) {

  switch(in->ss_family) {
      
  case AF_INET:
    ((struct sockaddr_in *)in)->sin_port = port;
    break;
      
  case AF_INET6:
    ((struct sockaddr_in6 *)in)->sin6_port = port;
    break;
  }

  return;
} 

uint16_t _check_port(uint32_t *opcode) {

  if(strncmp((char*)opcode,"PORT",OPCODE_LEN) != 0) {
    fprintf(stderr, "Invalid initial msg, expected PORT, got: %08x\n", *opcode);
    return 0;
  }
  
  return 1;
}

uint32_t _mygethostname(char *data) {

  uint32_t ret;
  int err;

  // hints
  struct addrinfo hints, *res;
  memset (&hints, 0, sizeof (hints));
  hints.ai_family = AF_INET;

  
  //debug
  fprintf(stderr, "getaddrinfo got %s from client\n", data);
  //debug
    
  if((err = getaddrinfo(data,NULL,&hints, &res)) != 0) { //Valgrind reports memory leak here
    fprintf(stderr, "getaddrinfo failed = %i\n", err);
    perror("* getaddrinfo error");
    freeaddrinfo(res); //but I free here
    return -1;
  }

  //debug
  /* struct sockaddr_storage *target; */
  /* target = (struct sockaddr_storage *) res->ai_addr; */
  /* print_sockaddr_udp_serv(target); */
  //debug

  // getting 32bit ip out of sockaddr-gross
  ret = htonl(((struct sockaddr_in*)(res->ai_addr))->sin_addr.s_addr); 
  
  //**cleanup
  freeaddrinfo(res); // and here?
  //**cleanup done

  return ret;
}


void _respond_udp(uint32_t *opcode, char *data, uint32_t *sen) {
  
  //debug
  fprintf(stderr, "received opcode: %08x\n", *opcode);
  //debug

  uint16_t add1, add2;

  if(*opcode == 0xCAFEBABE) { *sen = 0xDFACBABE; }
  
  else if(*opcode == 0x90) { strcpy((char*)sen,"POON"); } // "NOOP"
  
  else if(*opcode == 0x41444432) { // "ADD2"
    memcpy(&add1,data,2);
    memcpy(&add2,&data[2],2);
    *sen = add1 + add2;
  }
  
  else if(*opcode == 0x1) { // hostname

    *sen = _mygethostname(data);
    
  }

  else if(*opcode == 0xFFFFFFFF) { *sen = 0xFFFFFFFF; }// TERM

  
  else {
    fprintf(stderr, "! Invalid command from client\n");
    sen = NULL;
  }  
  
  *sen = htonl(*sen);
  
  return;
}


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

void converse_udp(uint16_t port, int ipDomain) {


  //debug
  fprintf(stderr, "\n---------------------------------------\n");
  fprintf(stderr, "converse_udp listening on %i:%hu\n",ipDomain,port);
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
  hints.ai_socktype = SOCK_DGRAM;
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
  print_sockaddr_udp_serv(target);
  //debug

  
  //**Make Socket
  int sockfd;
  if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0 ) {
    fprintf(stderr, "sockfd failed = %i\n", sockfd);
    perror("* socket error");
    return;
  }
  //**Socket success
  
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
  
  //**send/recv loop
  uint32_t sen, opcode;
  char rec[HOST_NAME_MAX+OPCODE_LEN] = {0};
  char data[HOST_NAME_MAX] = {0};
  struct sockaddr_storage client;
  socklen_t client_len = sizeof(client);
  uint16_t c_port;

  //get port number per protocol
  if(recvfrom(sockfd, &rec, HOST_NAME_MAX, 0, (struct sockaddr *)&client, &client_len) < 0) {
    fprintf(stderr, "* recvfrom error\n");
    perror("recvfrom");
    return;
  }
  
  //split rec into opcode and data
  memcpy(&opcode, rec, OPCODE_LEN);
  memcpy(data, &rec[OPCODE_LEN], HOST_NAME_MAX);
  
  
  if(_check_port(&opcode) == 0) {
    fprintf(stderr, "* check_port error\n");
    return;
  }

  //if we're here port command was good
  memcpy(&c_port, &rec[4], 2);
  _change_port(&client,c_port); //changing port number in client struct

  //debug
  fprintf(stderr, "changed port:\n");
  print_sockaddr_udp_serv(&client);
  //debug
  
  if(sendto(sockfd, &c_port, sizeof(uint16_t), 0, (struct sockaddr *)&client, client_len) < 0) {
    fprintf(stderr, "* sendto error\n");
    perror("sendto");
  }
  

  while(opcode != 0xFFFFFFFF) {

    if(recvfrom(sockfd, &rec, HOST_NAME_MAX, 0, (struct sockaddr *)&client, &client_len) < 0) {
      fprintf(stderr, "* recvfrom error\n");
      perror("recvfrom");
      return;
    }

    //split rec into opcode and data
    memcpy(&opcode, rec, OPCODE_LEN);
    opcode = ntohl(opcode);
    memcpy(data, &rec[OPCODE_LEN], HOST_NAME_MAX);
    
    _respond_udp(&opcode, data, &sen);
    
    //debug
    fprintf(stderr, "sending: %08x\n", sen);
    //debug
    
    if(sendto(sockfd, &sen, SERV_SEN_LEN, 0, (struct sockaddr *)&client, client_len) < 0) {
      fprintf(stderr, "* sendto error\n");
      perror("sendto");
    }
  }
  //**send/recs loop done
  
  //**close
  close(sockfd);
  //**close
  
  //**debug
  fprintf(stderr, "Done!\n");
  //**debug
  
  
  return;
}
