#ifndef JQR_3_1_15_SM_H
#define JQR_3_1_15_SM_H

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
int main_event_loop(char *fname);

#endif
