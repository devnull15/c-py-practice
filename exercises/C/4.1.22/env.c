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
  printf("%s\n", getenv("HOME"));

  printf("\n\nChanging value of ");
  printf("%s", envtochange);
  printf(" env var to ");
  printf("%s...\n", changeto);
  setenv("HOME", "/foo/bar", 1);
 
  printf("\n\nGetting value of ");
  printf("%s", envtochange);
  printf(" env var...\n");
  printf("%s\n", getenv("HOME"));
}
