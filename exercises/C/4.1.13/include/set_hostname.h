#ifndef SET_HOSTNAME_H
#define SET_HOSTNAME_H

#include "jqr_conditionals.h"

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
int set_hostname(char *new_hostname);

#endif
