#include "../include/jqr_conditionals.h"
#include <stdlib.h>
#include <ctype.h>

/* typedef struct _node */
/* { */
/*     int data; */
/*     struct _node *next; */
/* } node; */

/**
 * @brief apply the function parameter to each element of the array, 
 * creating a new array with the elements after applying the function. 
 * YOU MUST USE A FOR LOOP FOR THIS EXERCISE 
 * @param array array of ints to map function to
 * @param arrLen length of array
 * @param function function pointer to function to apply to elements of the array
 * 
 * @return a new array with each element being the result of applying the function 
 * to the same index element from the input array
*/
int *array_map(int array[], int arrLen, int (*function)(int value)) {
  int *ret = calloc(arrLen,sizeof(int));
  for(int i = 0; i < arrLen; i++) {
    ret[i] = (*function)(array[i]);
  }

  return ret;
}

/**
 * @brief sum the elements in a given linked list. YOU MUST USE A WHILE LOOP 
 * TO ITERATE OVER THE LINKED LIST FOR THIS EXERCISE
 * @param head pointer to first node in the linked list
 * 
 * @return the sum of all elements in the linked list
 */
int sum_list(node *head) {
  int sum = 0;
  while(head!=NULL) {
    sum += head->data;
    head = head->next;
  }
  
  return sum;
}

/**
 * @brief return the index of the first element larger than the given parameter
 * YOU MUST USE A DO-WHILE LOOP FOR THIS EXERCISE
 * @param array array to search
 * @param arrLen length of array
 * @param num number in question
 * 
 * @return the index of the first element in array that is larger than num or -1 if none exists
*/
int index_of_larger(int array[], int arrLen, int num) {
  int i = 0;
  do {
    if(array[i] > num) { return i; }
    i++;
  } while(i < arrLen);

  return -1;
}

/**
 * @brief determines if a value is even or odd. YOU MUST USE AN IF/ELSE STATEMENT 
 * FOR THIS EXERCISE
 * @param value integer to determine parity of
 * 
 * @return 0 if even 1 if odd
*/
int isEvenOdd(int value) {
  if(value%2 == 0) { return 0 ; }
  else { return 1; }
  return -1;
}

/**
 * @brief performs a calcuation based upon the characters in a string,
 * starting with 0,
 * add 1 if a character is an alpha character,
 * subtract 1 if a character is numeric,
 * subtract 2 if a character is a space,
 * add 2 if a character is a punctuation.
 * Example: for the string "L1nux R0cks!" would be:
 * 8 alpha characters (total 8), two numeric characters (total -2)
 * 1 space character (total -2), 1 punctuation (total 2)
 * 0 + 8 - 2 - 2 + 2 = 6 
 * YOU MUST USE AN IF/ELSE IF STATEMENT 
 * FOR THIS EXERCISE
 * @param a string on which to perform a calculation
 * 
 * @return the calculated value
*/
int reckoner(const char *in_string) {
  int ret = 0;
  int i = 0;
  char c = in_string[0];
  while(c != '\0') {
    if(isalpha(c)) { ret += 1; }
    else if(isdigit(c)) { ret -= 1; }
    else if(isspace(c)) { ret -= 2; }
    else if(ispunct(c)) { ret += 2; }
    else { return -1; }
    i++;
    c = in_string[i];
  }

  return ret;
}

/**
 * @brief determine if the word starts with 'a','b','c', or none of the three.
 *  YOU MUST USE A SWITCH STATEMENT FOR THIS EXERCISE
 * @param word string to determine starting character of
 * 
 * @return 1 is 'a',2 if 'b',3 if 'c', -1 for none
*/
int startsWith(char *word) {

  switch(word[0]) {
  case('a'):
    return 1;
    
  case('b'):
    return 2;
    
  case('c'):
    return 3;
    
  default:
    return -1;
  }
  
  return -1;
}


/**
 * @brief determine effective use of goto labels to construct a single exit point
 *  within a function.
 * Modify this template so that if malloc fails, a proper cleanup is performed. 
 *
 * int demo_teardown()
 * {
 *     int error_code;
 *     char * write_buffer = NULL;
 *     char * read_buffer = NULL;
 *
 *     write_buffer = xmalloc(50);
 *
 *     if (write_buffer == NULL)
 *     {
 *     }

 *     read_buffer = xmalloc(100);

 *     if (read_buffer == NULL)
 *     {
 *     }
 * }
 *
 *  YOU MUST PROPERLY USE A GOTO STATEMENT WITH A LABEL FOR THIS EXERCISE
 * @param word string to determine starting character of
 * @return 0 if success, 1 if fail
*/
int demo_teardown(void) {
  
  int error_code;
  char * write_buffer = NULL;
  char * read_buffer = NULL;
  
  write_buffer = xmalloc(50);
  
  if (write_buffer == NULL) {
    error_code = 1;
    goto EXIT_WRITE;
  }
  
  read_buffer = xmalloc(100);
  
  if (read_buffer == NULL){
    error_code = 1;
    goto EXIT_READ;
  }
  
  error_code = 0;
  free(read_buffer);
 EXIT_READ: free(write_buffer);
 EXIT_WRITE:
  return error_code;
  
}
