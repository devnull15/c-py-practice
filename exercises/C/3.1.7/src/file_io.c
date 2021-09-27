#include "../include/file_io.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#define BUFSIZE 4096


/**
 *  * @brief Create a file with a name of `filename`, or if a file with the given filename already exists,
 *   *        overwrites the file.  Write the contents of `data` into the file
 *    * 
 *     * @param filename string representing the name of the file that needs to be created
 *      * @param data data to be written to the newly created file
 *       * @return 0 on failure, 1 on success
 *        * 
 *         * Tasks used:
 *          *  3.1.7.c 
 *           *  3.1.7.e
 *            *  3.1.7.g
 *             */


int _print_file(FILE *f) {

  printf("print_file:\n");
  rewind(f);
  char c;
  while(EOF != (c = fgetc(f))) {
    printf("%c",c);
  }
  printf("\n");
  
  return 1;
}

int create_file(char *filename, char *data) {
	FILE *f = fopen(filename, "w");
	if (f == NULL) { return 0; }

	int ret = fwrite(data, 1, strlen(data), f);
	if(ret == 0) { return 0; }
	fclose(f);

	return 1;
}

/**
 *  * @brief Open the file at filename `filename`, read its contents, and modify it
 *   *        so that its characters are in reverse order (note: characters, not words),
 *    *        then close the file
 *     * 
 *      * @param filename string representing the name of the file that needs to be modified
 *       * @return 0 on failure, 1 on success
 *        * Tasks used: 
 *         *  3.1.7.a
 *          *  3.1.7.b
 *           *  3.1.7.d
 *            */

int modify_file(char *filename) {

	FILE *f = fopen(filename, "r+");
	if(f == NULL) { return 0; }
	char buf[BUFSIZE] = {0};
	char rev[BUFSIZE] = {0};
	int len = 0;
	char c;

	while( EOF !=  (c = fgetc(f)) ) { 
		buf[len] = c;
		len++;
		if(ferror(f) != 0) { return 0; }
	}
	
	for(int i = 0; i < len; i++) {
		rev[i] = buf[len-i-1];
	}
	
	rewind(f);
	int ret = fwrite(rev, 1, len, f);
	if(ret == 0) { return 0; }

	fclose(f);

	return 1;
}

/**
 *  * @brief Open the file at filename `filename`,then append `data` to the file, 
 *   *        before closing the file
 *    * 
 *     * @param filename string representing the name of the file that needs to be modified
 *      * @param data data to be appended to file
 *       * @return 0 on failure, 1 on success
 *        * Tasks used: 
 *         *  3.1.7.h
 *          */

int append_file(char *filename, char *data) {


	FILE *f = fopen(filename, "a");
	if(f == NULL) { return 0; }
	
	int ret = fwrite(data, 1, strlen(data), f);
	if(ret == 0) { return 0; }

	fclose(f);
	
	return 1;
}

/**
 *  * @brief Open the file at filename `filename`and insert `data` into the `index` 
 *   *        location of the file while preserving all data before and 
 *    *        after `index` before closing the file
 *     * 
 *      * @param filename string representing the name of the file that needs to be modified
 *       * @param data data to be appended to file
 *        * @param index index at which data is insterted
 *         * @return 0 on failure, 1 on success
 *          * Tasks used: 
 *           *  3.1.7.l
 *            */

int insert_file(char *filename, char *data, int index) {
  
  FILE *f = fopen(filename, "r+");
  if(f == NULL) { return 0; }
  
  char buf[BUFSIZE] = {0};
  int len = 0;
  fseek(f,index,SEEK_SET);
  
  while(EOF != (buf[len] = fgetc(f))) {
    //check if fgetc had error
    if(ferror(f) != 0) { return 0; }
    len++;
  }
  
  fseek(f, index,SEEK_SET);
  len = 0;
  
  for(size_t i = 0; i < strlen(data); i++) {
    fputc(data[i], f);
    //check if putc had error
    if(ferror(f) != 0) { return 0; }
  }

  // -1 to not copy the null into the file.
  for(size_t i = 0; i < strlen(buf)-1; i++) {
    fputc(buf[i],f);
  }

  //_print_file(f);
  
  fclose(f);
  
  return 1;
}

/**
 *  * @brief Open the file at filename `filename`and return the
 *   *         first occurence of `needle` within the file (case sensitive)
 *    * 
 *     * @param filename string representing the name of the file to be searched
 *      * @param needle character to find in file
 *       * @return -1 if needle does not occur in the file, otherwise return the index
 *        *          of the first occurence of needle
 *         * Tasks used: 
 *          *  3.1.7.k
 *           */

int find_char_file(char *filename, char needle) {
  FILE *f = fopen(filename, "r");
  if(f == NULL) { return 0; }

  int i = 0;
  char c;
  while(needle != (c = fgetc(f))) {
    if(c == EOF) { return -1; }
    i++;
  }

  fclose(f);
  
  return i;
  
}

/**
 *  *  @brief Open file at `filename`, use the struct `stat` to print file 
 *   *         information to the console, including file size and file permissions.
 *    *  @param filename name of file whose info to print
 *     *
 *      * Your expected output will be three lines long:
 *       *  Statistics for <filename>\n
 *        *  File Size: <file size> bytes\n
 *         *  File Permissions: <file permissions>
 *          *
 *           * You should give your file permissions in the same format used by `ls -l`:
 *            * first character should be 'd' if it's a directory
 *             * then use 'r', 'w', and 'x' to designate read/write/execute for user(owner),
 *              * group, and other. If the permission is not set use '-'
 *               *
 *                * Ex: If a file is rwx by everyone "-rwxrwxrwx"
 *                 * If a file is only readable by the owner "-r--------"
 *                  * If a directory if read/write by owner and read by everyone else "drw-r--r--"
 *                   *
 *                    * If you are unable to get the file statistics you should print 
 *                     * "Error retrieving statistics for <filename>\n" and return
 *                      *
 *                       * Tasks used: 
 *                        *  3.1.7.f
 *                         *  3.1.7.j
 *                          *  3.1.7.k
 *                          **/
void file_info(char *filename) {

  //I stole some of this from:
  //https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
  // and then modified it.
  
  char fpath [BUFSIZE];
  //getcwd(fpath, sizeof(fpath));
  /* fprintf(stderr,fpath); */
  /* fprintf(stderr,"\n"); */
  strcat(fpath,filename);
  /* fprintf(stderr,fpath); */
  /* fprintf(stderr,"\n"); */
  
  struct stat fileStat;
  if(stat(fpath, &fileStat) < 0) {
    printf("Error retrieving statistics for %s\n",filename);
    return;
  }

  printf("Statistics for %s\n", filename);
  printf("File Size: %li bytes\n",fileStat.st_size);

  char perms[BUFSIZE];
  strcat(perms, (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
  strcat(perms, (fileStat.st_mode & S_IRUSR) ? "r" : "-");
  strcat(perms, (fileStat.st_mode & S_IWUSR) ? "w" : "-");
  strcat(perms, (fileStat.st_mode & S_IXUSR) ? "x" : "-");
  strcat(perms, (fileStat.st_mode & S_IRGRP) ? "r" : "-");
  strcat(perms, (fileStat.st_mode & S_IWGRP) ? "w" : "-");
  strcat(perms, (fileStat.st_mode & S_IXGRP) ? "x" : "-");
  strcat(perms, (fileStat.st_mode & S_IROTH) ? "r" : "-");
  strcat(perms, (fileStat.st_mode & S_IWOTH) ? "w" : "-");
  strcat(perms, (fileStat.st_mode & S_IXOTH) ? "x" : "-");
  strcat(perms, "\n");

  // fprintf(stderr, "perms: %s", perms);
  printf("File Permissions: %s", perms);
  
  return;
}

/**
 *  *  @brief Delete file at `filename`,   
 *   *  @param filename name of the file to delete
 *    *  @return 1 if file successfully deleted, 0 otherwise
 *     *  Tasks used:
 *      *    3.1.7.i
 *      **/
int delete_file(char *filename) {
  if(remove(filename) == -1) { return 0; }
  return 1;
}

