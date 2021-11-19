#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Creates and returns a new int pointer after initializing the pointer to NULL. JQR Items tested:
 *  - 3.1.10.a
 * 
 * @return An int pointer that is set to NULL
*/
int *fresh_int_pointer() {
  int *ret = 0;
  return ret;
}

/**
 * @brief Returns the value pointed at by the parameter. JQR Items tested:
 *  - 3.1.10.b
 * @param value integer whose address to return
 * 
 * @return int pointer of address of value
*/
int get_value(int *address) {
  return *address;
}

/**
 * @brief Prints the address where the variable pointed to by the pass argument. 
 * JQR Items tested:
 *  - 3.1.10.c
 * @param address pointer to integer in which to print the address.
*/
void print_address(int *address) {
  printf("%p",address);
  return;
}

/**
 * @brief Assigns value to the integer pointed to by address
 * JQR Items tested:
 *  - 3.1.10.d
 * @param address pointer to integer whose value to change
 * @param value value to assign
*/
void set_value(int *address, int value) {
  *address = value;
  return;
}

/**
 * @brief call the function `function` with `arg` as its arguments. JQR Items tested: - 3.1.10.e
 *
 * @param function function to call
 * @param arg argument to pass into function 
*/
void call_func(void (*function)(void *arg), void *arg) {
  (*function)(arg);
  return;
}

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
int fold_left(int array[], int arrLen, int (*binOp)(int acc, int curr), int start) {
  int acc = 0;
  for( int i = start; i < arrLen; i++) {
    acc = (*binOp)(acc,array[i]);
  }
  return acc;
}
