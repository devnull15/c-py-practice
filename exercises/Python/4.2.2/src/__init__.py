#!/usr/bin/env python3

from singleton_decorator import singleton

""" Docstrings for 3.2.2 types demonstrate """

def integer():
    """Declare a variable as an integer and return it"""
    x = 0
    return x


def floater():
    """Declare a variable as a float value and return it"""
    x = 20.15
    return x

def strings():
    """Declare a variable as a string value and return it"""
    x = "hi"
    return x

def lists():
    """Declare a list with four values in it and return it"""
    x = [1,2,3,4]
    return x 

def multi_list():
    """Create a multi dimensional list and return it"""
    x = [[1,2],[3,4]]
    return x

def dictionary():
    """Create a dictionary and return it"""
    x = {"x":1, "y":2}
    return x

def toople():
    """Declare a tuple with four values in it and return it"""
    x = (1,2,3,4)
    return x


@singleton
class MyLogger:
    """
    Implement a class that will function as a logger.

    Decorate this class with a singleton
    """
    def __init__(self, file_logger=False, file_name=None):
        """
        Initialize the logger.

        The logger should just write messages to stdout.
        """
        pass

    def log(self, msg):
        """
        Write msg to stdout.
        """
        print(msg)
        pass

    

def singleton():
    """

    Code a simple class decorated by a singleton, then add
    two instances of the class to a dictionary, return the
    length of the dictionary, which should be one (1)

    """

    x = dict(one=MyLogger(), two=MyLogger())
    return len(x)
