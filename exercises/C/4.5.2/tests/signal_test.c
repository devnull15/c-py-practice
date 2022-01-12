#ifndef JQR_352_SIGNALS_TEST_H
#define JQR_352_SIGNALS_TEST_H

#define _GNU_SOURCE

#include <sys/types.h>
#include <unistd.h>

#include "CUnit/Basic.h"

#include "../include/signals.h"

void
test_signal_setup(void)
{
    CU_ASSERT_PTR_NOT_NULL_FATAL(&signal_action_setup);

    struct sigaction sigStruct = {0};

    /*Test for FALSE is checking for 0 on success*/
    CU_ASSERT_FALSE(signal_action_setup(&sigStruct))
    /*Test for TRUE checks for error on invalid input*/
    CU_ASSERT_TRUE(signal_action_setup(NULL));
}

void
test_signal_handler(void)
{
    CU_ASSERT_PTR_NOT_NULL_FATAL(&assignVal);
    int check = 0;
    pid_t procID = getpid();
    /*Raise Signal SIGUSR1 and GLOBAL_SIGNAL_NUMBER should equal 1*/
    kill(procID, SIGUSR1);
    assignVal(&check);
    CU_ASSERT_EQUAL(1, check);

    /*Raise Signal SIGUSR2 and GLOBAL_SIGNAL_NUMBER should equal 2*/
    kill(procID, SIGUSR2);
    assignVal(&check);
    CU_ASSERT_EQUAL(2, check);
}

#endif