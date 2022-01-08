#ifndef JQR_352_INTERRUPTS_H
#define JQR_352_INTERRUPTS_H

/**
 * @brief Using inline assembly invoke a system call to write the provided
 * message to stdout.
 * 
 * @param message The message to write
 * @return int The number of bytes written.
 */
int do_write(void* message);

/**
 * @brief Using inline assembly or signals, create a debug interrupt.
 * 
 */
void do_int3(void);

#endif
