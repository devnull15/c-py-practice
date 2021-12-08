#!/usr/bin/env python3

"""JQR Items Covered

3.2.10
title: Demonstrate the ability to parse command-line arguments using built-in 
functionality

Brief: Add function parameters and your own implementation to complete the 
       functions. Test your solution against the tests suite using
       "from tests import run_tests; run_tests()"
"""

def parse_cli(args):
    """
    Using the argparse module, create a parser the handle the command line 
    arguments, args.

    You should take 2 positional arguments, x which is the base and y which 
    is the exponent.

    You should also take an optional argument "-m" or "--modulus" which is
    the modulus

    All arguments will be integer values.

    args: args A list of command line arguments
    return: (x ^ y) mod m
    """
