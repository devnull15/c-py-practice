#include "../include/helper.h"
#include <sys/un.h>

/**
 * @brief Connects to a `SOCK_STREAM` Unix socket specified and sends
 * specified number of bytes from supplied buffer, then closes the socket
 *
 * YOU MUST USE THE FOLLOWING FUNCTIONS FOR THIS EXERCISE:
 *     socket();
 *     connect();
 *     send();
 *
 *  YOU MUST USE THE FOLLOWING STRUCTURES FOR THIS EXERCISE:
 *     struct sockaddr_un;
 *
 * YOU MUST USE PROPER TYPECASTING USING:
 *     (struct sockaddr*)
 *
 * @param: 
 *     `path` path to unix socket
 *     `buffer` address of the data to send
 *     `len` number of bytes from buffer to send
 *
 * @return: Return bytes sent
*/

int send_unix(char *path, char *buffer, int len) {

  int bytes_sent;
  int err;
  
  //debug
  fprintf(stderr, "\n---------------------------------------\n");
  // fprintf(stderr, "send_unix sending %s(%i) to %s\n",buffer,len,path);
  fprintf(stderr, "send_unix sending length (%i) to %s\n",len,path);
  //debug
  
  //sockaddr_un
  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, path, strlen(path));
  
  
  //**Make Socket
  int sockfd;
  if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 ) {
    fprintf(stderr, "sockfd failed = %i\n", sockfd);
    perror("* socket error");
    return sockfd;
  }
  //**Socket success

  
  //**Connect
  if((err = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr))) < 0) {
    fprintf(stderr, "connect failed = %i\n", err);
    perror("* connect error");
    return err;
  }
  //**Connect done
  

  //**send
  if((bytes_sent = send(sockfd, buffer, len,0)) < 0) {
    fprintf(stderr, "* send error\n");
    perror("send");
  }
  //**send done

    fprintf(stderr, "Here\n");

  
  return bytes_sent;
}
