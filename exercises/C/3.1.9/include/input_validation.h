#ifndef JQR_319_FILE_IO_H
#define JQR_319_FILE_IO_H

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>



/*

   JQR Items Covered:

       3.1.9:
      title: 'Demonstrate the ability to perform data validation:'
      condition: C
      standard: B
      sub_items:
        3.1.9.a: Validating input received matches input expected
*/


/**
 * @brief validates that a string contains a valid 32 bit signed integer (ascii-represenation of the integer)
 *
 * @param a pointer to a null-terminated string (character array) representing the input to be evaluated
 * @return 0 on invalid input, 1 on valid input
 * Tasks used:
 *  3.1.9.a
 */
int valid_int(char* data);

/**
 * @brief validate that a string is composed of only the alphanumeric character set (lowercase and numbers, only).
 *
 * @param a pointer to a null-terminated string (character array) representing the input to be evaluated
 * @return 0 on invalid input, 1 on valid input
 * Tasks used:
 *  3.1.9.a
 */
int allowed_char(char* data);

#endif
