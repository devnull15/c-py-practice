#include <jqr_conditionals.h>
#include <stdlib.h>
#include <ctype.h>

int *array_map(int array[], int arrLen, int (*function)(int value)) {
  int *ret = calloc(arrLen,sizeof(int));
  if(ret != NULL) {
    for(int i = 0; i < arrLen; i++) {
      ret[i] = (*function)(array[i]);
    }
  }

  return ret;
}

int sum_list(node *head) {
  int sum = 0;
  while(NULL != head) {
    sum += head->data;
    head = head->next;
  }
  
  return sum;
}

int index_of_larger(int array[], int arrLen, int num) {
  int i = 0;
  do {
    if(array[i] > num) { return i; }
    i++;
  } while(i < arrLen);

  return -1;
}

int isEvenOdd(int value) {
  return (value%2==0) ? 0 : 1;
}

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
  read_buffer = NULL;
 EXIT_READ:
  free(write_buffer);
  write_buffer = NULL;
 EXIT_WRITE:
  return error_code;
  
}
