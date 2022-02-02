#!/usr/bin/env python

import unittest
import junit_xml
import random
import pathlib
import importlib.util 
import os
import sys

#loads library from environmental variable or from a relative path
#https://docs.python.org/3/library/importlib.html#importing-a-source-file-directly

TEST_FILE = os.environ.get("TEST_FILE")

TEST_SUITE_OUTPUT = "suite_results.xml"
TEST_SUITE_NAME = "3.2.3"

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


class Adding(unittest.TestCase):

    def setUp(self):
        self.a = random.randint(1, 3000000000)
        self.b = random.randint(1, 3000000000)

    def correct(self):
        return self.a + self.b

    def test_adding(self):
        self.assertEqual(student.add_me(self.a, self.b), self.correct())


class Subtracting(unittest.TestCase):

    def setUp(self):
        self.a = random.randint(1, 3000000000)
        self.b = random.randint(1, 3000000000)

    def correct(self):
        return self.a - self.b

    def test_subtracting(self):
        self.assertEqual(student.subtract_me(self.a, self.b), self.correct())


class Multiplying(unittest.TestCase):

    def setUp(self):
        self.a = random.randint(1, 3000000000)
        self.b = random.randint(1, 3000000000)

    def correct(self):
        return self.a * self.b

    def test_mulitplying(self):
        self.assertEqual(student.multiply_me(self.a, self.b), self.correct())


class Dividing(unittest.TestCase):

    def setUp(self):
        self.a = random.randint(1, 3000000000)
        self.b = random.randint(1, 3000000000)

    def correct(self):
        return self.a / self.b

    def test_dividing(self):
        self.assertEqual(student.divide_me(self.a, self.b), self.correct())
        self.assertIsNone(student.divide_me(1, 0))


class Modulo(unittest.TestCase):

    def setUp(self):
        self.a = random.randint(1, 3000000000)
        self.b = random.randint(1, 3000000000)

    def correct(self):
        return self.a % self.b

    def test_modding(self):
        self.assertEqual(student.mod_me(self.a, self.b), self.correct())

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
