#!/usr/bin/env python3

import unittest
import random
import pathlib
import junit_xml
import importlib.util 
import os
import sys

import os
import random


#loads library from environmental variable or from a relative path
#https://docs.python.org/3/library/importlib.html#importing-a-source-file-directly

TEST_FILE = os.environ.get("TEST_FILE")

TEST_SUITE_OUTPUT = "suite_results.xml"
TEST_SUITE_NAME = "3.2.6"

if TEST_FILE and \
        pathlib.Path(TEST_FILE).is_file():
    LIB_PATH = pathlib.Path(
            os.environ.get("TEST_FILE"))
    if LIB_PATH.is_dir():
        LIB_PATH = LIB_PATH / "__init__.py"
else:
    LIB_PATH = pathlib.Path.cwd() / pathlib.Path(__file__)
    LIB_PATH = LIB_PATH.parent.parent / "src/__init__.py"

SPEC = importlib.util.spec_from_file_location(
        LIB_PATH.name, LIB_PATH)
student = importlib.util.module_from_spec(SPEC)
sys.modules["student"] = student
SPEC.loader.exec_module(student)

class Testing(unittest.TestCase):
    def seed_random(self):
        random.seed(os.urandom(128))

    def test_bookstore_negative(self):
        try:
            student.Bookstore(-11)
        except AssertionError:
            pass
        else:
            print("Should not be able to create bookstore w/ negative books")
            self.fail()

    def test_bookstore(self):
        try:
            student.Bookstore(random.randint(10, 20))
        except Exception as exc:
            print(exc)
            self.fail()
    
    def test_bookstore_zero(self):
        try:
            student.Bookstore(0)
        except AssertionError:
            print("This should be allowed")
            self.fail()

    def test_buy_negative(self):
        store = student.Bookstore(random.randint(10, 20))
        try:
            store.buy(-10)
        except AssertionError:
            pass
        else:
            print("Should not be able to buy negative books")
            self.fail()

    def test_buy_zero(self):
        store = student.Bookstore(random.randint(10, 20))
        try:
            store.buy(0)
        except AssertionError:
            print("This should be allowed")
            self.fail()
        

    def test_buy_books(self):
        total_books = random.randint(10, 20)
        store = student.Bookstore(total_books)
        buy = random.randint(1, 10)
        while (total_books - buy) >= 0: 
            total_books -= buy
            store.buy(buy)
            buy = random.randint(1, 10)
        try:
            store.buy(buy)
        except student.NotEnoughBooksError:
            pass
        else:
            print("Did not raise NotEnoughBooksError")
            self.fail()
            


def export_suite(result):
    test_cases = []
    for index in range(result.__dict__['testsRun']):
        cur_test_case = junit_xml.TestCase("{}-{}".format(TEST_SUITE_NAME,index),"test.class",0.0,"","")
        test_cases.append(cur_test_case)
    index = 0;
    for failure in result.failures + result.errors:
        test_cases[index].add_failure_info(message=failure[1])
        index += 1
    test_suite = [junit_xml.TestSuite(TEST_SUITE_NAME,test_cases)]
    with open(TEST_SUITE_OUTPUT, 'w') as f:
        junit_xml.TestSuite.to_file(f, test_suite, prettyprint=True)

def run_tests():
    x = unittest.main(exit=False,verbosity=3)
    export_suite(x.result)

if __name__ == '__main__':
    run_tests()
