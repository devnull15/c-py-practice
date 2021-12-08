#include <stdio.h>
#include <stdlib.h>

#define DATA_ARRAY_SIZE 65535
/**
 * @brief This is the main loop that will run the interpreter
 *
 * We are going to be writing an interpreter for an esolang called BFe.
 * The language consist of 8 valid characters which each correspond to
 * a single operation.
 *
 * The commands are executed sequentially, with some exceptions: an instruction
 * pointer begins at the first command, and each command it points to is
 * executed, after which it normally moves forward to the next command. The
 * program terminates when the instruction pointer moves past the last command.
 *
 * You will write code to implement the state machine that executes this
 * language. The machine model consists of the program and instruction pointer,
 * as well as a one-dimensional array of at least 30,000 byte cells initialized
 * to zero; a movable data pointer (initialized to point to the leftmost byte of
 * the array); and two streams of bytes for input and output (most often
 * connected to a keyboard and a monitor respectively, and using the ASCII
 * character encoding).
 *
 * Supported language commands
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * >	increment the data pointer (to point to the next cell to the right).
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * <	decrement the data pointer (to point to the next cell to the left).
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * +	increment (increase by one) the byte at the data pointer.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * -	decrement (decrease by one) the byte at the data pointer.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * .	output the byte at the data pointer.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * ,	accept one byte of input, storing its value in the byte at the data
 * pointer.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * [	if the byte at the data pointer is zero, then instead of moving
 * the instruction pointer forward to the next command, jump it forward to the
 * command after the matching ] command.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * ]	 unconditional jump to the corresponding [ command
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *
 * `[` and `]` match the same way that parentheses usually do: each [ matches
 * exactly one ] and vice versa, the [ comes first, and there can be no
 * unmatched [ or ] between the two.
 *
 * @param fname The file that contains the BFe program.
 * @return 0 if executed successfully a negative number otherwise
 */

void _ip_print(char inst, int i) {
  fprintf(stderr,"%i:%c\n",i,inst);
  return;
}

void _dp_print(char *dp) {
  fprintf(stderr, "dp: %#0hhx >%#0hhx %#0hhx\n", dp[-1],dp[0],dp[1]);
}

void inc_dp(char **dp) {
  *dp += 1;
}

void dec_dp(char **dp) {
  *dp -= 1;
}

void inc_data(char *dp) {
  *dp += 1;

}

void dec_data(char *dp) {
  *dp -= 1;

}

void output(char *dp) {
  printf("%c",*dp);
  fprintf(stderr,"%c",*dp);
}

void input(char *dp) {
  *dp = fgetc(stdin);
}

int l_bracket(char *dp, FILE *ip) {
  char inst;
  if(*dp != 0) { // if data pointer is not zero do nothing and continue
    return 1;
  }
  else { // else set ip to inst after ']'
    int stack = 0;
    while(']' != (inst = fgetc(ip)) && !stack) {
      if(feof(ip)) { // eof reached, program fails syntax rule: '[...]'
	return 0;
      }
      if(inst=='[') { stack++; }
      if(inst==']') { stack--; }
      if(stack<0) { return 0; } // too many ], syntax error
    }
  }
  return 1;
}
  
int r_bracket(char *dp, FILE *ip) {
  char inst;
  /* if(*dp == 0) { // if data pointer is zero do nothing and continue */
  /*   return 1; */
  /* } */

  /* else { //else go back to matching '[' */
  int stack = 0;
  fseek(ip,-2,SEEK_CUR);
  while('[' != (inst = fgetc(ip)) && !stack) { // rewind until we get to matching '['
    if(ftell(ip) == 0) { return 0; } // beginning of file, syntax error
    if(inst==']') { stack++; } // account for nested brackets
    if(inst=='[') { stack--; }
    if(stack<0) { return 0; } // too many [, syntax error
    fseek(ip,-2,SEEK_CUR);
  }
    //}
  ungetc(inst,ip); // put [ back in stream
  
  return 1;
}

void err(char inst, int i) {
  fprintf(stderr, "!invalid instruction at %i: %c:%#hhx\n", i,inst,inst);
  return;
}


int main_event_loop(char *fname) {

  // open program file
  FILE *ip = fopen(fname, "r"); // this is technically the instruction pointer
  if(ip == NULL) { return 0; }

  //data pointer
  char *dp = calloc(DATA_ARRAY_SIZE,sizeof(char));
  
  // read in each command one by one (char by char).
  char inst; //current instruction
  int i = 0; //counts # of instructions executed
  
  while(EOF != (inst = fgetc(ip))) {
    
    //debug
    _ip_print(inst,i);
    //debug

    switch(inst) {

    case '>':
      inc_dp(&dp);
      break;

    case '<':
      dec_dp(&dp);
      break;

    case '+':
      inc_data(dp);
      break;

    case '-':
      dec_data(dp);
      break;

    case '.':
      output(dp);
      break;

    case ',':
      input(dp);
      break;
      
    case '[':
      if (!l_bracket(dp,ip)) {
	err(inst,i);
	return -1;
      }
      //debug
      inst = fgetc(ip);
      fprintf(stderr,"char after [: %#hhx:%c\n",inst,inst);
      ungetc(inst,ip);      
      //debug

      break;
      
    case ']':
      if (!r_bracket(dp, ip)) {
	err(inst,i);
	return -1;
      }
      //debug
      inst = fgetc(ip);
      fprintf(stderr,"char after ]: %#hhx:%c\n",inst,inst);
      ungetc(inst,ip);
      //debug
      
      break;
      
    default:
      err(inst,i);
    }

    //debug
    _dp_print(dp);
    fprintf(stderr, "------------------------------\n");
    //debug
    
    i++;
    
  }
  
  return 0;
}
