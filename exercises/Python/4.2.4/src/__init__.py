#!/usr/bin/env python3

import os

"""JQR Items Covered

3.2.4
title: Demonstrate the ability to perform file management operations in Python

sub_items:
  a: open an existing file
  b: read data from a file
  c: parse data from a file
  d: write data to a file
  e: modify data in a file
  f: close an open file
  g: print file information to the console
  h: create a new file
  i: append data to an existing file
  j: delete a file
  k: determine the size of a file
  l: determine location within a file
  m: insert data into an existing file


Brief: Add function parameters and your own implementation to complete
       the functions. Test your solution against the tests suite using
       "from tests import run_tests; run_tests()"
"""


def reading(f01):
    '''Open a file and return the contents'''
    f = open(f01, 'r')
    ret = f.read()
    f.close()
    return ret


def parsing(f01):
    '''
    Parse data from the file and add all strings that start with "k" to a list and return it.
    '''
    ret = []
    with open(f01, 'r') as f:
        for line in f:
            for w in line.split():
                if w.startswith('k'):
                    ret.append(w)
    return ret


def writing(f01, quote):
    '''Write the quote to a file'''
    f = open(f01, 'w')
    ret = f.write(quote)
    f.close()
    return



def modifying(f01):
    '''
    Modify text in file named modify.txt. Replace the letter "f" with the word "cyber"
    '''
    mod = ""
    with open(f01, 'r') as f:
        mod = f.read()
    mod = mod.replace('f','cyber')
    with open(f01, 'w') as f:
        f.write(mod)
    return


def console(f01):
    '''Return file information using os'''
    print(f01)
    print(os.path.getsize(f01))
    print(os.path.getmtime(f01))
    return


def create(filename):
    '''Create a file with the provided filename'''
    with open(filename,'w') as f:
        pass
    return

def append(f01, str1):
    '''Append string to the file'''
    with open(f01,'a') as f:
        f.write(str1)
    return


def delete(filename):
    '''Delete this file'''
    os.remove(filename)
    return


def size(filename):
    '''Return the filesize of the given file'''
    return os.path.getsize(filename)


def path(filename):
    '''Return the path of the file'''
    return os.path.abspath(filename)


def insert(f01):
    '''Insert the word cyber every other word in the file'''
    mod = ""
    with open(f01, 'r') as f:
        mod = f.read()
    mod = mod.replace(' ',' cyber ')
    with open(f01, 'w') as f:
        f.write(mod)
    return
