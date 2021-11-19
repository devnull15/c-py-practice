#ifndef JQR_317_FILE_IO_H
#define JQR_317_FILE_IO_H

#include <stdint.h>
#include <float.h>
#include <stdio.h>
#include <unistd.h>

/*
 
   JQR Items Covered: 

       3.1.7:
      title: 'Demonstrate the ability to perform file management operations in C:'
      condition: C
      standard: B
      sub_items:
        3.1.7.a: Open an existing file
        3.1.7.b: Read data from a file
        3.1.7.c: Write data to a file
        3.1.7.d: Modify data to a file
        3.1.7.e: Close an open file
        3.1.7.f: Print file information to the console
        3.1.7.g: Create a new file
        3.1.7.h: Append data to an existing file
        3.1.7.i: Delete a file
        3.1.7.j: Determine the size of a file
        3.1.7.k: Determine the location within a file
        3.1.7.l: Insert data into an existing file
 
*/


/**
 * @brief Create a file with a name of `filename`, or if a file with the given filename already exists,
 *        overwrites the file.  Write the contents of `data` into the file
 * 
 * @param filename string representing the name of the file that needs to be created
 * @param data data to be written to the newly created file
 * @return 0 on failure, 1 on success
 * 
 * Tasks used:
 *  3.1.7.c 
 *  3.1.7.e
 *  3.1.7.g
 */

int create_file(char *filename, char *data);

/**
 * @brief Open the file at filename `filename`, read its contents, and modify it
 *        so that its characters are in reverse order (note: characters, not words),
 *        then close the file
 * 
 * @param filename string representing the name of the file that needs to be modified
 * @return 0 on failure, 1 on success
 * Tasks used: 
 *  3.1.7.a
 *  3.1.7.b
 *  3.1.7.d
 */

int modify_file(char *filename);

/**
 * @brief Open the file at filename `filename`,then append `data` to the file, 
 *        before closing the file
 * 
 * @param filename string representing the name of the file that needs to be modified
 * @param data data to be appended to file
 * @return 0 on failure, 1 on success
 * Tasks used: 
 *  3.1.7.h
 */

int append_file(char *filename, char *data);

/**
 * @brief Open the file at filename `filename`and insert `data` into the `index` 
 *        location of the file while preserving all data before and 
 *        after `index` before closing the file
 * 
 * @param filename string representing the name of the file that needs to be modified
 * @param data data to be appended to file
 * @param index index at which data is insterted
 * @return 0 on failure, 1 on success
 * Tasks used: 
 *  3.1.7.l
 */

int insert_file(char *filename, char *data, int index);

/**
 * @brief Open the file at filename `filename`and return the
 *         first occurence of `needle` within the file (case sensitive)
 * 
 * @param filename string representing the name of the file to be searched
 * @param needle character to find in file
 * @return -1 if needle does not occur in the file, otherwise return the index
 *          of the first occurence of needle
 * Tasks used: 
 *  3.1.7.k
 */

int find_char_file(char *filename, char needle);

/**
 *  @brief Open file at `filename`, use the struct `stat` to print file 
 *         information to the console, including file size and file permissions.
 *  @param filename name of file whose info to print
 *
 * Your expected output will be three lines long:
 *  Statistics for <filename>\n
 *  File Size: <file size> bytes\n
 *  File Permissions: <file permissions>
 *
 * You should give your file permissions in the same format used by `ls -l`:
 * first character should be 'd' if it's a directory
 * then use 'r', 'w', and 'x' to designate read/write/execute for user(owner),
 * group, and other. If the permission is not set use '-'
 *
 * Ex: If a file is rwx by everyone "-rwxrwxrwx"
 * If a file is only readable by the owner "-r--------"
 * If a directory if read/write by owner and read by everyone else "drw-r--r--"
 *
 * If you are unable to get the file statistics you should print 
 * "Error retrieving statistics for <filename>\n" and return
 *
 * Tasks used: 
 *  3.1.7.f
 *  3.1.7.j
 *  3.1.7.k
**/
void file_info(char *filename);

/**
 *  @brief Delete file at `filename`,   
 *  @param filename name of the file to delete
 *  @return 1 if file successfully deleted, 0 otherwise
 *  Tasks used:
 *    3.1.7.i
**/
int delete_file(char *filename);

#endif
