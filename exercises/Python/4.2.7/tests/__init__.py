#!/usr/bin/env python

import unittest
import random
import pathlib
import junit_xml
import importlib.util
import string
import os
import sys

#loads library from environmental variable or from a relative path
#https://docs.python.org/3/library/importlib.html#importing-a-source-file-directly

TEST_FILE = os.environ.get("TEST_FILE")

TEST_SUITE_OUTPUT = "suite_results.xml"
TEST_SUITE_NAME = "3.2.7"

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


class countAlphaNumeric(unittest.TestCase):

    def randomStringGenerator(self, type, length):
        if type == 0:
            avail_char = string.ascii_letters + string.digits

        if type == 1:
            avail_char = string.ascii_letters + string.digits + string.punctuation

        randomString = ''.join(random.choice(avail_char)
                               for i in range(length))

        return randomString

    def setUp(self):
        self.test_string_1 = self.randomStringGenerator(
            0, random.randint(1, 50))
        self.test_string_2 = self.randomStringGenerator(
            1, random.randint(1, 50))

    def correct(self, input):
        digits = 0
        letters = 0
        output = []

        for char in input:
            if char.isdigit():
                digits += 1

            elif char.isalpha():
                letters += 1

            else:
                pass

        output.append(digits)
        output.append(letters)

        return output

    def test_alphanumeric(self):
        self.assertEqual(student.ifElifElseStatement(
            self.test_string_1), self.correct(self.test_string_1))

    def test_mixedalphanumericsymbols(self):
        self.assertEqual(student.ifElifElseStatement(
            self.test_string_2), self.correct(self.test_string_2))


class isPrime(unittest.TestCase):

    def setUp(self):
        self.test_int = random.randint(0, 20000)

    def correct(self, number):
        if number > 1:
            for i in range(2, number // 2):
                if number % i == 0:
                    return False

            else:
                return True
        else:
            return False

    def test_prime(self):
        self.assertEqual(self.correct(13), student.ifElseStatement(13))

    def test_notprime(self):
        self.assertEqual(self.correct(6), student.ifElseStatement(6))

    def test_random(self):
        self.assertEqual(self.correct(self.test_int),
                         student.ifElseStatement(self.test_int))


class noDivThree(unittest.TestCase):

    def setUp(self):
        self.no_div_three = []
        for i in range(random.randint(0, 3000)):
            self.no_div_three.append(random.randint(0, 20000))

    def correct(self, list):
        return [n for n in list if n % 3 != 0]

    def test_ifStatement(self):
        self.assertEqual(self.correct(self.no_div_three),
                         student.ifStatement(self.no_div_three))


class squares(unittest.TestCase):

    def setUp(self):
        self.rand_list = []
        for i in range(random.randint(0, 3000)):
            self.rand_list.append(random.randint(0, 20000))

    def correct(self, list):
        for i in list:
            i = i * i
        return list

    def test_forLoop(self):
        self.assertTrue(self.correct(self.rand_list),
                        student.forLoop(self.rand_list))


class countDown(unittest.TestCase):

    def setUp(self):
        self.rand_int = random.randint(5, 20)

    def correct(self, num):
        output = []
        while num >= 5:
            output.append(num)
            num = num - 1
        return output

    def test_whileLoop(self):
        self.assertEqual(self.correct(self.rand_int), student.whileLoop(self.rand_int))


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
