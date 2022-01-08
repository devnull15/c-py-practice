#include "../include/interrupts.h"
#include <stddef.h>
#include <stdio.h>

/**
 * @brief Using inline assembly invoke a system call to write the provided
 * message to stdout.
 * 
 * @param message The message to write
 * @return int The number of bytes written.
 */
int do_write(void* message) {
  void *ptr = NULL;
  int cnt = 0;

  /* __asm__("xor %rbx, %rbx;" */
  /* 	  "loop:" */
  /* 	  "cmp $10, %rbx;" */
  /* 	  "je done;" */
  /* 	  "inc %rbx;" */
  /* 	  "jmp loop;" */
  /* 	  "done:"); */
  /* __asm__("movl %%ebx, %0;" : "=r" (cnt)); */
  

  //WHY DOES THIS WORK???
  /* __asm__("mov %1, %%rsi;" : "=S" (ptr) : "r" (message) ); */
  /* __asm__("xor %rbx, %rbx;" */
  /* 	  "loop:" */
  /* 	  "mov (%rsi), %r12;" // %r12 = message[ptr] */
  /* 	  "cmp $0x00, %r12;" */
  /* 	  "je done;" */
  /* 	  "mov $1, %rdx;" // count = 1 */
  /* 	  "mov $2, %rdi;" // fd = stdout */
  /* 	  "mov $1, %rax;" // write(fd, buf, count) */
  /* 	  "syscall;" */
  /* 	  "inc %rbx;" */
  /* 	  "inc %rsi;"); */
  /* __asm__( */
  /* 	  "jmp loop;" */
  /* 	  "done:"); */
  /* __asm__("movl %%ebx, %0;" : "=r" (cnt)); */

  
  __asm__("mov %1, %%rsi;" : "=S" (ptr) : "r" (message) );
  __asm__(//strlen
	  "xor %rcx, %rcx;"
	  "dec %rcx;"
	  "xor %al, %al;"
	  "cld;"
	  "repnz scasb;"
	  "not %rcx;"
	  "dec %rcx;"
	  //strlen
	  "mov %rcx, %rbx;" // count = strlen
	  "mov %rcx, %rdx;" // count = strlen
  	  "mov $1, %rdi;" // fd = stdout
  	  "mov $1, %rax;" // write(fd, buf, count)
	  "syscall;");
  __asm__("movl %%ebx, %0;" : "=r" (cnt));

  return cnt;
}

/**
 * @brief Using inline assembly or signals, create a debug interrupt.
 * 
 */
void do_int3(void) {
  __asm__("int3;");
  
}
