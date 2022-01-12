#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>

// Used to check if the proper signal handling was done
extern volatile sig_atomic_t GLOBAL_SIGNAL_NUMBER;

/**
 * @brief This will be the function used to handle the signals.
 *
 * The following signals will modify the global variable "GLOBAL_SIGNAL_NUMBER"
 * - SIGUSER1: This signal should set GLOBAL_SIGNAL_NUMBER to 1
 * - SIGUSER2: This signal should set GLOBAL_SIGNAL_NUMBER to 2
 *
 * @param signal The signal that was raised
 */
void signal_handler(int signal);

/**
 * @brief This function is used to set up the signal handling.
 *
 * @param sigStruct A pointer to an empty sigaction struct that will be used
 * for signal handling
 * @return int 0 on success any other number on failure
 */
int signal_action_setup(struct sigaction *sigStruct);

/**
 * @brief This function is for testing the setting of GLOBAL_SIGNAL NUMBER
 * in a shared object environment. Simply assign the value in
 * GLOBAL_SIGNAL_NUMBER to check and return.
 *
 * @param check This is the variable that GLOBAL_SIGNAL_NUMBER will be assigned
 * to
 */
void assignVal(int *check);

#endif
