#ifndef JQR_318_FUNCTION_H
#define JQR_318_FUNCTION_H

/**
 * @brief Set the value of val based on the value of timer.
 * 
 * If timer is an even number then set val to 0.
 * If timer is an odd number then set val to 1.
 * 
 * Satisfies 
 * JQR 3.1.8.b
 * JQR 3.1.8.c
 * JQR 3.1.8.d
 * 
 * @param timer Value that determines the value of val
 * @param val Used to return the value based on timer
 */
void pass_by_ref(int timer, int* val);

/**
 * @brief Return the value stored at val.
 * 
 * JQR 3.1.8.e
 * 
 * @param val Pointer to the value we want to return
 * @return int Value stored at val
 */
int dereference(int* val);

/**
 * @brief Get a character from stdin.
 * 
 * @return char The character
 */
char get_char(void);

/**
 * @brief Return the nth fibonacci number.
 * 
 * Your function must be recursive.
 * 
 * JQR 3.1.8.h
 * 
 * @param n What fibonacci number to return
 * @return int the nth fibonacci number
 */
int fib_recursive(int n);

/**
 * @brief Call the provided function.
 * 
 * JQR 3.1.8.g
 * 
 * @param function The function to call
 * @return int The return value from the called function
 */
int call_my_function(int (*function)(void));


#endif
