#include <stddef.h>
#include "../include/signals.h"


volatile sig_atomic_t GLOBAL_SIGNAL_NUMBER = 0;

/**
 * @brief This will be the function used to handle the signals.
 *
 * The following signals will modify the global variable "GLOBAL_SIGNAL_NUMBER"
 * - SIGUSER1: This signal should set GLOBAL_SIGNAL_NUMBER to 1
 * - SIGUSER2: This signal should set GLOBAL_SIGNAL_NUMBER to 2
 *
 * @param signal The signal that was raised
 */
void signal_handler(int signal) {
  if(signal == SIGUSR1) { GLOBAL_SIGNAL_NUMBER = 1; }
  else if (signal == SIGUSR2) { GLOBAL_SIGNAL_NUMBER = 2; }
}

/**
 * @brief This function is used to set up the signal handling.
 *
 * @param sigStruct A pointer to an empty sigaction struct that will be used
 * for signal handling
 * @return int 0 on success any other number on failure
 */
int signal_action_setup(struct sigaction *sigStruct) {
  if(sigStruct == NULL) { return -1; }
  sigStruct->sa_handler = &signal_handler;
  sigaction(SIGUSR1, sigStruct, NULL);
  sigaction(SIGUSR2, sigStruct, NULL);
  return 0;
}

/**
 * @brief This function is for testing the setting of GLOBAL_SIGNAL NUMBER
 * in a shared object environment. Simply assign the value in
 * GLOBAL_SIGNAL_NUMBER to check and return.
 *
 * @param check This is the variable that GLOBAL_SIGNAL_NUMBER will be assigned
 * to
 */
void assignVal(int *check) {
  *check = GLOBAL_SIGNAL_NUMBER;
}
