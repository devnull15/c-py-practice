#ifndef JQR_3110_POINTERS_H
#define JQR_3110_POINTERS_H

#include <stdint.h>
#include <float.h>
#include <stdio.h>
#include <unistd.h>

/*
   JQR Items Covered: 

    condition: C
    standard: B
    sub_items:
        3.1.10.a: declaring an integer pointer
        3.1.10.b: dereferincing a variable to get its value
        3.1.10.c: printing the address of a variable
        3.1.10.d: assigning a value to a pointer
        3.1.10.e: make use of a function pointer to call another function
        3.1.10.f: make effective use of a pointer arithmetic to traverse an array
    title: 'Demonstrate the following skills using pointers:'

 */

/**
 * @brief Creates and returns a new int pointer after initializing the pointer to NULL. JQR Items tested:
 *  - 3.1.10.a
 * 
 * @return An int pointer that is set to NULL
*/
int *fresh_int_pointer();

/**
 * @brief Returns the value pointed at by the parameter. JQR Items tested:
 *  - 3.1.10.b
 * @param value integer whose address to return
 * 
 * @return int pointer of address of value
*/
int get_value(int *address);

/**
 * @brief Prints the address where the variable pointed to by the pass argument. 
 * JQR Items tested:
 *  - 3.1.10.c
 * @param address pointer to integer in which to print the address.
*/
void print_address(int *address);

/**
 * @brief Assigns value to the integer pointed to by address
 * JQR Items tested:
 *  - 3.1.10.d
 * @param address pointer to integer whose value to change
 * @param value value to assign
*/
void set_value(int *address, int value);

/**
 * @brief call the function `function` with `arg` as its arguments. JQR Items tested: - 3.1.10.e
 *
 * @param function function to call
 * @param arg argument to pass into function 
*/
void call_func(void (*function)(void *arg), void *arg);

// Ref https://www.scala-lang.org/api/current/scala/collection/immutable/List.html#foldLeft[B](z:B)(op:(B,A)=%3EB):B
/**
 * @brief Applies the function defined by binOp function pointer to elements in array from left to right.
 *        JQR Items tested: - 3.1.10.e - 3.1.10.f
 * @param array array of ints to apply the binary operation to
 * @param arrLen length of array
 * @param binOp binary operation to perform on consecutive ints from array, where acc is the accumulated 
 *        total and curr is the current element
 * @param start start value to apply to binOp along with the first element in the array
 * 
 * @return the result of inserting op between consecutive elements of this sequence, going left to right 
 * with the start value start on the left
 * @example array = [1,2,3,4] -> foldLeft(array,a+b,0) = 10
 */
int fold_left(int array[], int arrLen, int (*binOp)(int acc, int curr), int start);

#endif
