#include "../include/helper.h"
#include <unistd.h>

/**
 * @brief Connects to the supplied IP and sends supplied number of bytes of
 * data from supplied input, closes connection after sending
 *
 * YOU MUST USE THE FOLLOWING FUNCTIONS FOR THIS EXERCISE:
 *     sethostname();
 *     
 * @param: `new_hostname` valid hostname character string of 63 characters
 * consisting of alpha numeric characters and the hyphen '-'
 *
 * @return: Return 0 on success and  negative value for errors
*/
int set_hostname(char *new_hostname) {
  
  //debug
  fprintf(stderr, "\n---------------------------------------\n");
  fprintf(stderr, "set_hostname to %s\n",new_hostname);
  //debug
  
  if(sethostname(new_hostname,strlen(new_hostname))<1) {
    fprintf(stderr,"\nsethostname failed\n");
    perror("sethostname");
    return -1;
  }
  
  return 0;
}
