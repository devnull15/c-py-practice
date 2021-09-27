#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

/**
 * @brief validates that a string contains a valid 32 bit signed integer (ascii-represenation of the integer)
 *
 * @param a pointer to a null-terminated string (character array) representing the input to be evaluated
 * @return 0 on invalid input, 1 on valid input
 * Tasks used:
 *  3.1.9.a
 */
int valid_int(char* data) {
  if(data[0] == '\0') { return 0; }
  if(!isdigit(data[0]) && data[0] != '-') { return 0; }
  for(size_t i = 1; i < strlen(data); i++) {
    if(!isdigit(data[i])) { return 0; }
  }
  
  if(atol(data) > INT_MAX || atol(data) < INT_MIN) { return 0; }

  return 1;
}

/**
 * @brief validate that a string is composed of only the alphanumeric character set (lowercase and numbers, only).
 *
 * @param a pointer to a null-terminated string (character array) representing the input to be evaluated
 * @return 0 on invalid input, 1 on valid input
 * Tasks used:
 *  3.1.9.a
 */
int allowed_char(char* data) {

  if(data[0] == '\0') { return 0; }
  
  int i = 0;
  while(data[i] != '\0') {
    if(!isalnum(data[i])) { return 0; }
    if(!isdigit(data[i]) && !islower(data[i])) { return 0; }

    i++;
  }

  return 1;
}

