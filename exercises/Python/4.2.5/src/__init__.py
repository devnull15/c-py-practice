#!/usr/bin/env python
'''Return multiple values as a tuple'''
def returnValues(n):

    #print(n)
    return tuple(n)

'''Create a function that takes user input and return the value'''
def userInput():
    return input()


'''
Write a function using recursion to get the length of the string.

MUST USE RECURSION
'''
def recursion(word):
    if not word:
        return 0
    else:
        return recursion(word[:-1]) + 1

