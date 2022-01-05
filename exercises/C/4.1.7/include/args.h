#ifndef JQR_316_ARGUMENTS_H
#define JQR_316_ARGUMENTS_H

#include <stdio.h>

/**
 * @brief We're going to basically create our own main function.
 * 
 * Arguments are treated just like they are in the traditional main().
 * 
 * Print each entry in argv on its own line.
 * 
 * @param argc Length of argv
 * @param argv Array of arguments
 * @param envp Array of environment variables, NULL terminated like in main()
 * @return size_t The length of envp
 */
size_t my_main(int argc, char** argv, char** envp);

#endif