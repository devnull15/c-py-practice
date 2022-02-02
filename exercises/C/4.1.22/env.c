#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;


int main() {
  char **ptr = environ;
  char *envtochange = "HOME";
  char *changeto = "/foo/bar";
  printf("Getting environment vars...\n");
  while(NULL != *ptr) {
    printf("%s\n", *ptr);
    ptr++;
  }

  printf("\n\nGetting value of ");
  printf("%s", envtochange);
  printf(" env var...\n");
  char *ret = getenv("HOME");
  if(NULL != ret) {
    printf("%s\n", ret);
  }
  else {
    fprintf(stderr, "!!! env variable not found.\n");	
  }


  printf("\n\nChanging value of ");
  printf("%s", envtochange);
  printf(" env var to ");
  printf("%s...\n", changeto);
  if(-1 == setenv("HOME", "/foo/bar", 1)) {
    fprintf(stderr, "!!! error with setenv");
  }
  
 
  printf("\n\nGetting value of ");
  printf("%s", envtochange);
  printf(" env var...\n");
  ret = getenv("HOME");
  if(NULL != ret) {
    printf("%s\n", ret);
  }
  else {
    fprintf(stderr, "!!! env variable not found.\n");	
  }
}
