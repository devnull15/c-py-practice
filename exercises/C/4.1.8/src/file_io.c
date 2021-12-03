#include "../include/file_io.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#define BUFSIZE 4096


void _print_file(FILE *f) {

  printf("print_file:\n");
  rewind(f);
  char c;
  while(EOF != (c = fgetc(f))) {
    printf("%c",c);
  }
  printf("\n");
}

int create_file(char *filename, char *data) {
  int ret = 1;
  FILE *f = fopen(filename, "w+");
  if (f == NULL) {
    ret = 0;
    goto EXIT_FUNC;
  }

  int err = fwrite(data, 1, strlen(data), f);
  if(err == 0) {
    ret = 0;
    goto EXIT_FUNC;
  }

 EXIT_FUNC:
  if(f != NULL){
    fclose(f);
    f = NULL;
  }
  
  return ret;
}


int modify_file(char *filename) {

  int ret = 1;
  FILE *f = fopen(filename, "r+");
  if(f == NULL) {
    ret = 0;
    goto EXIT_FUNC;
  }

  char buf[BUFSIZE] = {0};
  char rev[BUFSIZE] = {0};
  int len = 0;
  char c;
  
  while( EOF !=  (c = fgetc(f)) ) { 
    buf[len] = c;
    len++;
    if(len > BUFSIZE) {
      ret = 0;
      goto EXIT_FUNC;
    }
    if(ferror(f) != 0) {
      ret = 0;
      goto EXIT_FUNC;
    }
  }
	
  for(int i = 0; i < len; i++) {
    rev[i] = buf[len-i-1];
  }
  
  rewind(f);
  int err = fwrite(rev, 1, len, f);
  if(err == 0) {
    ret = 0;
    goto EXIT_FUNC;
  }
  
 EXIT_FUNC:
  if(f != NULL){
    fclose(f);
    f = NULL;
  }
  
  return ret;
}


int append_file(char *filename, char *data) {

  int ret = 1;
  
  FILE *f = fopen(filename, "a");
  if(f == NULL) {
    ret = 0;
    goto EXIT_FUNC;
  }
  
  int err = fwrite(data, 1, strlen(data), f);
  if(err == 0) {
    ret = 0;
    goto EXIT_FUNC;
  }
  
 EXIT_FUNC:
  if(f != NULL){
    fclose(f);
    f = NULL;
  }
  
  return ret;
}


int insert_file(char *filename, char *data, int index) {
  
  int ret = 1;
  
  FILE *f = fopen(filename, "r+");
  if(f == NULL) {
    ret = 0;
    goto EXIT_FUNC;
  }
  
  char buf[BUFSIZE] = {0};
  int len = 0;
  if(fseek(f,index,SEEK_SET) == -1) {
    ret = 0;
    goto EXIT_FUNC;
  }
  
  while(EOF != (buf[len] = fgetc(f))) {
    //check if fgetc had error
    if(ferror(f) != 0) {
      ret = 0;
      goto EXIT_FUNC;
    }
    len++;
    if(len > BUFSIZE) {
      ret = 0;
      goto EXIT_FUNC;
    }
  }
  
  if(fseek(f,index,SEEK_SET) == -1) {
    ret = 0;
    goto EXIT_FUNC;
  }

  len = 0;
  
  for(size_t i = 0; i < strlen(data); i++) {
    fputc(data[i], f);
    //check if putc had error
    if(ferror(f) != 0) {
      ret = 0;
      goto EXIT_FUNC;
    }
  }

  // -1 to not copy the null into the file.
  for(size_t i = 0; i < strlen(buf)-1; i++) {
    fputc(buf[i],f);
  }

  //_print_file(f);

 EXIT_FUNC:
  if(f != NULL){
    fclose(f);
    f = NULL;
  }
  
  return ret;
}


int find_char_file(char *filename, char needle) {
  int i = 0;
  FILE *f = fopen(filename, "r");
  if(f == NULL) {
    i = 0;
    goto EXIT_FUNC;
  }

  char c;
  while(needle != (c = fgetc(f))) {
    if(c == EOF) {
      i = -1;
      goto EXIT_FUNC;
    }
    i++;
  }
  
 EXIT_FUNC:
  if(f != NULL){
    fclose(f);
    f = NULL;
  }
  
  return i;
  
}

void file_info(char *filename) {

  //I stole some of this from:
  //https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
  // and then modified it.
    
  struct stat fileStat;
  if(stat(filename, &fileStat) < 0) {
    printf("Error retrieving statistics for %s\n",filename);
    return;
  }

  printf("Statistics for %s\n", filename);
  printf("File Size: %li bytes\n",fileStat.st_size);

  printf("File Permissions: ");
  printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
  printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
  printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
  printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
  printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
  printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
  printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
  printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
  printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
  printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
  printf("\n");
    
  return;
}

int delete_file(char *filename) {
  if(remove(filename) == -1) { return 0; }
  return 1;
}

