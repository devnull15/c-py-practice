#!/usr/bin/env python3

import unittest
import random
import pathlib
import importlib.util 
import os
import junit_xml
import sys

import random
import string
import enum
import traceback
import socket
import inspect
import types

from functools import partial






#loads library from environmental variable or from a relative path
#https://docs.python.org/3/library/importlib.html#importing-a-source-file-directly

TEST_FILE = os.environ.get("TEST_FILE")

TEST_SUITE_OUTPUT = "suite_results.xml"
TEST_SUITE_NAME = "3.2.9"

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


##### built-in randomization functions #####

def get_random_str(start=7, stop=15):
	return ''.join(random.choice(string.ascii_letters) for __ in range(random.randrange(start,stop)))

def get_random_list(length_start=1, length_stop=10, value_start=1, value_stop=20):
    return [random.randrange(value_start,value_stop) for __ in range(random.randrange(length_start,length_stop))]

def get_random_float(start=1, stop=10):
	return random.uniform(start,stop)

def get_random_int(start=1, stop=10, step=1):
	return random.randrange(start,stop,step)


def is_called(caller, called, *args, **kwargs):
    """
    Tests if function 'caller' calls 'called'
    https://stackoverflow.com/questions/12013399/in-python-determine-if-a-function-calls-another-function
    :param caller: callable to check
    :param called: callable that should be called
    :param *args: arguments for the 'caller' argument
    :param **kwargs: keyword arguments for the 'caller' argument
    :return: bool
    """
    def trace_calls(call_list, frame, event, arg):
        if event != 'call':
            return
        call_list.append(frame.f_code)

    traces = []
    old = sys.gettrace()
    sys.settrace(partial(trace_calls, traces))
    try:
        caller(*args, **kwargs)
    finally:
        sys.settrace(old)

    try:
        idx = traces.index(called.__code__)
    except ValueError:
        return False

    if idx and traces[idx-1] == caller.__code__:
        return True
    return False

##### end built-in functions #####

# pylint: disable-msg=R0903
class Test():
    """Used for Testing purposes"""
    pass
# pylint: enable-msg=R0903


class Suite(enum.Enum):
    """ Suite enumerations """
    Py_FUNDAMENTALS = 0x00001
    Py_FILE_IO = 0x00010
    Py_CONCURRENCY = 0x00100
    Py_NETWORKING = 0x01000
    Py_OOP = 0x10000

    def __or__(self, other):
        return self.value | other.value




# pylint: disable-msg=R0913
# pylint: disable-msg=R0904
class CustomUnitTest(unittest.TestCase):
    """The Testing suite class for unit test assertions"""
    question = 0
    tag = None
    out = None
    _resultForDoCleanups = None

    def tearDown(self):
        if hasattr(self, '_outcome'):  # Python 3.4+
            # these 2 methods have no side effects
            result = self.defaultTestResult()
            self._feedErrorsToResult(result, self._outcome.errors)
        else:  # Python 3.2 - 3.3 or 2.7
            result = getattr(self,
                             '_outcomeForDoCleanups',
                             self._resultForDoCleanups)
        error = self.list2reason(result.errors)
        failure = self.list2reason(result.failures)
        okay = not error and not failure

        # demo:   report short info immediately (not important)
        if not okay:
            text = ('ERROR', error) if error else ('FAIL', failure)
            msg = [x for x in text[1].split('\n')[1:] if x.split("File")][0].split(",", 1)[1]
            print("FAIL, ", '"', msg.replace(",", " -"), '"', file=self.out)

    def list2reason(self, exc_list):
        """List conversion"""
        if exc_list and exc_list[-1][0] is self:
            return exc_list[-1][1]
        return None

    def custom_test_print(self, function_name, extra_tag, jqr_items):
        """
        needs to output in this order
        JRQ_Tag, JQR_Item, Test Name, Function Name, Assert #, Pass/fail
        """
        test_info = self.id().split('.')

        print(self.tag.name, end="", file=self.out)
        if extra_tag:
            if isinstance(extra_tag, list):
                for tag in extra_tag:
                    print(" ", tag.name, end="", file=self.out)
            else:
                print(" ", extra_tag.name, end="", file=self.out)
        print(", ", end="", file=self.out)
        for item in jqr_items:
            print(item + " ", end="", file=self.out)
        print(",", end="", file=self.out)
        print(self.id(), ",", end="", file=self.out)
        print(function_name, ",", end="", file=self.out)

    def custom_test_assert(self, value, extra_tag, jqr_items, msg=None):
        """
        /* Assert that expression is TRUE (non-zero) */
        """
        self.custom_test_print(value, extra_tag, jqr_items)
        self.assertTrue(value, msg=msg)
        print("PASS, ", file=self.out)
        return

    def custom_test_assert_equal(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert equal
        """
        self.custom_test_print("custom_test_assert_equal", extra_tag, jqr_items)
        self.assertEqual(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_not_equal(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert not equal
        """
        self.custom_test_print("custom_test_assert_not_equal", extra_tag, jqr_items)
        self.assertNotEqual(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_true(self, value, extra_tag, jqr_items, msg=None):
        """
        Assert true
        """
        self.custom_test_print("custom_test_assert_true", extra_tag, jqr_items)
        self.assertTrue(value, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_false(self, value, extra_tag, jqr_items, msg=None):
        """
        Assert false
        """
        self.custom_test_print("custom_test_assert_false", extra_tag, jqr_items)
        self.assertFalse(value, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_is(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert is
        """
        self.custom_test_print("custom_test_assert_is", extra_tag, jqr_items)
        self.assertIs(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_is_not(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert is not
        """
        self.custom_test_print("custom_test_assert_is_not", extra_tag, jqr_items)
        self.assertIsNot(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_is_none(self, value, extra_tag, jqr_items, msg=None):
        """
        Assert is None
        """
        self.custom_test_print("custom_test_assert_is_none", extra_tag, jqr_items)
        self.assertIsNone(value, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_is_not_none(self, value, extra_tag, jqr_items, msg=None):
        """
        Assert not None
        """
        self.custom_test_print("custom_test_assert_is_not_none", extra_tag, jqr_items)
        self.assertIsNotNone(value, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_in(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert in
        """
        self.custom_test_print("custom_test_assert_in", extra_tag, jqr_items)
        self.assertIn(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_not_in(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert
        """
        self.custom_test_print("custom_test_assert_not_in", extra_tag, jqr_items)
        self.assertNotIn(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_is_instance(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert is an instance of
        """
        self.custom_test_print("custom_test_assert_is_instance", extra_tag, jqr_items)
        self.assertIsInstance(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_not_is_instance(self,
                                    value_1,
                                    value_2,
                                    extra_tag,
                                    jqr_items):
        """
        Assert is not an instance of
        """
        self.custom_test_print("custom_test_assert_not_is_instance", extra_tag, jqr_items)
        self.assertNotIsInstance(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_raises(self,
                           extra_tag,
                           jqr_items,
                           exc,
                           fun,
                           *args,
                           **kwds):
        """
        Assert
        """
        self.custom_test_print("custom_test_assert_raises", extra_tag, jqr_items)
        self.assertRaises(exc, fun, *args, **kwds)
        print("PASS, ", file=self.out)

    def custom_test_assert_raises_regex(self,
                                 extra_tag,
                                 jqr_items,
                                 exc,
                                 reg_expr,
                                 fun,
                                 *args,
                                 **kwds):
        """
        Assert regex raise
        """
        self.custom_test_print("custom_test_assert_raises_regex", extra_tag, jqr_items)
        self.assertRaisesRegex(exc, reg_expr, fun, *args, **kwds)
        print("PASS, ", file=self.out)

    def custom_test_assert_warns(self,
                          extra_tag,
                          jqr_items,
                          warn,
                          fun,
                          *args,
                          **kwds):
        """
        Assert warns
        """
        self.custom_test_print("custom_test_assert_warns", extra_tag, jqr_items)
        self.assertWarns(warn, fun, *args, **kwds)
        print("PASS, ", file=self.out)

    def custom_test_assert_warns_regex(self,
                                extra_tag,
                                jqr_items,
                                warn,
                                reg_expr,
                                fun,
                                *args,
                                **kwds):
        """
        Assert regex warning
        """
        self.custom_test_print("custom_test_assert_warns_regex", extra_tag, jqr_items)
        self.assertWarnsRegex(warn, reg_expr, fun, *args, **kwds)
        print("PASS, ", file=self.out)

    def custom_test_assert_logs(self, extra_tag, jqr_items, logger, level, msg=None):
        """
        Assert logs
        """
        self.custom_test_print("custom_test_assert_logs", extra_tag, jqr_items)
        self.assertLogs(logger, level, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_almost_equal(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert approximately equal
        """
        self.custom_test_print("custom_test_assert_almost_equal", extra_tag, jqr_items)
        self.assertAlmostEqual(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_not_almost_equal(self,
                                     value_1,
                                     value_2,
                                     extra_tag,
                                     jqr_items, msg=None):
        """
        Assert not approximately equal
        """
        self.custom_test_print("custom_test_assert_not_almost_equal", extra_tag, jqr_items)
        self.assertNotAlmostEqual(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_greater(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert greater than
        """
        self.custom_test_print("custom_test_assert_greater", extra_tag, jqr_items)
        self.assertGreater(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_greater_equal(self,
                                  value_1,
                                  value_2,
                                  extra_tag,
                                  jqr_items, msg=None):
        """
        Assert greater than equal
        """
        self.custom_test_print("custom_test_assert_greater_equal", extra_tag, jqr_items)
        self.assertGreaterEqual(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_less(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert less than
        """
        self.custom_test_print("custom_test_assert_less", extra_tag, jqr_items)
        self.assertLess(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_less_equal(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert less than equal
        """
        self.custom_test_print("custom_test_assert_less_equal", extra_tag, jqr_items)
        self.assertLessEqual(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_regex(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert is regex
        """
        self.custom_test_print("custom_test_assert_regex", extra_tag, jqr_items)
        self.assertRegex(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_not_regex(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert not a regex
        """
        self.custom_test_print("custom_test_assert_not_regex", extra_tag, jqr_items)
        self.assertNotRegex(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_count_equal(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert equal count
        """
        self.custom_test_print("custom_test_assert_count_equal", extra_tag, jqr_items)
        self.assertCountEqual(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_multi_line_equal(self,
                                     value_1,
                                     value_2,
                                     extra_tag,
                                     jqr_items, msg=None):
        """
        Assert multi-line is equal
        """
        self.custom_test_print("custom_test_assert_multi_line_equal", extra_tag, jqr_items)
        self.assertMultiLineEqual(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_sequence_equal(self,
                                   value_1,
                                   value_2,
                                   extra_tag,
                                   jqr_items, msg=None):
        """
        Assert sequence is equal
        """
        self.custom_test_print("custom_test_assert_sequence_equal", extra_tag, jqr_items)
        self.assertSequenceEqual(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_list_equal(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert list is equal
        """
        self.custom_test_print("custom_test_assert_list_equal", extra_tag, jqr_items)
        self.assertListEqual(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_tuple_equal(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert tuple is equal
        """
        self.custom_test_print("custom_test_assert_tuple_equal", extra_tag, jqr_items)
        self.assertTupleEqual(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_set_equal(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert set is equal
        """
        self.custom_test_print("custom_test_assert_set_equal", extra_tag, jqr_items)
        self.assertSetEqual(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_assert_dict_equal(self, value_1, value_2, extra_tag, jqr_items, msg=None):
        """
        Assert dictionary is equal
        """
        self.custom_test_print("custom_test_assert_dict_equal", extra_tag, jqr_items)
        self.assertDictEqual(value_1, value_2, msg=msg)
        print("PASS, ", file=self.out)

    def custom_test_fail(self, extra_tag, jqr_items, msg=None):
        """
        Assert fail
        """
        self.custom_test_print("custom_test_fail", extra_tag, jqr_items)
        self.fail(msg)

    def custom_test_pass(self, extra_tag, jqr_items):
        """
        Assert pass
        """
        self.custom_test_print("custom_test_pass", extra_tag, jqr_items)
        print("PASS, ", file=self.out)

# pylint: enable-msg=R0913
# pylint: enable-msg=R0904



# pylint: disable-msg=W0703
class IPv4AddressTestSuite(CustomUnitTest):
    tag = Suite.Py_OOP

    def setUp(self):
        # generate a low IP address
        low_parts = [get_random_int(0, 256) for __ in range(4)]
        low_parts[0] = get_random_int(0, 128)
        self.lo_str = '.'.join(list(map(str, low_parts)))
        self.lo_int = low_parts[0] * 256**3 + low_parts[1] * 256**2 + low_parts[2] * 256 + low_parts[3]
        self.lo_class = student.IPv4Address(self.lo_str)

        # generate a high IP address
        hi_parts = [get_random_int(0, 256) for __ in range(4)]
        hi_parts[0] = get_random_int(128, 256)
        self.hi_str = '.'.join(list(map(str, hi_parts)))
        self.hi_int = hi_parts[0] * 256**3 + hi_parts[1] * 256**2 + hi_parts[2] * 256 + hi_parts[3]
        self.hi_class = student.IPv4Address(self.hi_str)

        # generate an IP address equal to the low IP address
        self.eq_class = student.IPv4Address(self.lo_str)

    def test_init_str_and_repr_methods(self):
        '''
        Ensure initialization handles all cases
        '''
        # generate a valid IP address
        ip_parts = [get_random_int(0, 256) for __ in range(4)]
        ip_address = '.'.join(list(map(str, ip_parts)))

        # genereate an invalid IP address
        ip_parts[get_random_int(0,4)] = get_random_int(256, 512)
        ip_address_2 = '.'.join(list(map(str, ip_parts)))

        # test all valid options
        for option in [get_random_int(0, 4294967296),
                        str(get_random_int(0, 4294967296)),
                        ip_address]:
            with self.subTest(init_input=option):
                test_class = student.IPv4Address(option)
                if isinstance(option, str) and '.' in option:
                    # if the input is in IP address format, test the __str__ method
                    self.custom_test_assert_equal(str(test_class), option,
                                     Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9g"],
                                     msg="string representation should be in octet format")
                else:
                    # if the input is an integer, test the __repr__ mehod
                    answer = str(option)
                    self.custom_test_assert_equal(repr(test_class), answer,
                                     Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9g"],
                                     msg="debug representation should be in integer format")

        # test all invalid options
        for option in [-1,
                        "-1",
                        "4294967296",
                        ip_address_2,
                        get_random_list(),
                        '192.168.0',
                        '127.0.0.1.24',
                        '12.34.56.78/24']:
            with self.subTest(init_input=option):
                test_class = student.IPv4Address(option)
                func = random.choice([str, repr])
                self.custom_test_assert_equal(func(test_class), "None",
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9g"],
                                 msg="invalid IP addresses should return None for both string and debug representation")

    def test_get_set_ip_from_any_variable_name(self):
        '''
        Ensure getters and setters work regardless of variable name
        '''
        # generate a random IP address
        ip_address = [get_random_int(0, 256) for __ in range(4)]
        ip_address = '.'.join(list(map(str, ip_address)))

        # test all valid options
        for option in [get_random_int(0, 4294967296),
                       str(get_random_int(0, 4294967296)),
                       ip_address]:
            with self.subTest(setter_input_value=option):
                # generate a random variable to set
                variable_name = "self.eq_class.{}".format(get_random_str())
                function = "{} = {}".format(variable_name, repr(option))
                exec(function)

                # test off of __str__ or __repr__ as needed
                error_msg = "Variable {} is not linked to IP address value".format(variable_name)
                if isinstance(option, str) and '.' in option:
                    self.custom_test_assert_equal(str(self.eq_class), option,
                                     Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9i"],
                                     msg=error_msg)
                else:
                    self.custom_test_assert_equal(repr(self.eq_class), str(option),
                                     Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9i"],
                                     msg=error_msg)

        # test invalid option
        option = []
        with self.subTest(setter_input_value=option):
            # generate a random variable to set
            variable_name = "self.eq_class.{}".format(get_random_str())
            function = "{} = {}".format(variable_name, repr(option))
            exec(function)

            # test results should be 'None'
            error_msg = "Variable {} is not linked to IP address value".format(variable_name)
            self.custom_test_assert_equal(str(self.eq_class), 'None',
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9i"],
                                 msg=error_msg)

        # test random getter
        variable_name = "self.eq_class.{}".format(get_random_str())
        with self.subTest(getter_variable_name=variable_name):
            # ensure the variable exists
            failure = False
            try:
                ip_as_integer = eval(variable_name)
            except AttributeError:
                failure = True
            if failure:
                self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="Variable {} not found".format(variable_name))

            # ensure the variable is linked to the IP address value
            error_msg = "Variable {} is not IP address value".format(variable_name)
            self.custom_test_assert_equal(repr(self.eq_class), str(ip_as_integer),
                             Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                             msg=error_msg)

    def test_is_multicast(self):
        '''
        Test the is_multicast property works appropriately
        '''
        # generate IP address parts
        ip_parts = [get_random_int(0, 256) for __ in range(4)]

        # generate an invalid IP address
        ip_parts[0] = get_random_int(-255, -1)
        ip_address_0 = '.'.join(list(map(str, ip_parts)))

        # generate a valid lower IP address
        ip_parts[0] = get_random_int(0, 224)
        ip_address_c = '.'.join(list(map(str, ip_parts)))

        # generate a valid multicast IP address
        ip_parts[0] = get_random_int(224, 240)
        ip_address_d = '.'.join(list(map(str, ip_parts)))

        # generate a valid higher IP address
        ip_parts[0] = get_random_int(240, 256)
        ip_address_e = '.'.join(list(map(str, ip_parts)))

        # test all possible responses
        addresses = [(ip_address_0, False),
                     (ip_address_c, False),
                     (ip_address_d, True),
                     (ip_address_e, False)]
        for option, response in addresses:
            with self.subTest(input_argument=option):
                test_class = student.IPv4Address(option)
                error_msg = "'{}' should be labeled as '{}'".format(option, response)
                self.custom_test_assert_is_instance(test_class.is_multicast, bool,
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9h"],
                                 msg="student.IPv4Address.is_multicast method should be a boolean property")
                self.custom_test_assert_equal(test_class.is_multicast, response,
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9h"],
                                 msg=error_msg)

    def test_is_private(self):
        '''
        Test the is_private method works appropriately
        '''
        # test all public ip addresses
        for option in [-1,
                       get_random_int(0, 167772160),
                       get_random_int(184549376, 2130706432),
                       get_random_int(2147483648, 2886729728),
                       get_random_int(2887778304, 3232235520),
                       get_random_int(3232301056, 4294967296)]:
            with self.subTest(input_argument=option):
                test_class = student.IPv4Address(option)
                is_ip_address_private = test_class.is_private()
                self.custom_test_assert_is_instance(is_ip_address_private, bool,
                                      Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9h"],
                                      msg="function 'is_private' should return a bool")
                self.custom_test_assert_false(is_ip_address_private,
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9h"],
                                 msg="invalid or public IP addresses should return false")

        # test all private ip addresses
        for option in [get_random_int(167772160, 184549376),
                       get_random_int(2130706432, 2147483648),
                       get_random_int(2886729728, 2887778304),
                       get_random_int(3232235520, 3232301056)]:
            with self.subTest(input_argument=option):
                test_class = student.IPv4Address(option)
                is_ip_address_private = test_class.is_private()
                self.custom_test_assert_is_instance(is_ip_address_private, bool,
                                      Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9h"],
                                      msg="function 'is_private' should return a bool")
                self.custom_test_assert_true(is_ip_address_private,
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9h"],
                                 msg="private IP addresses should return true")

    def test_addition(self):
        '''
        Test the __add__ and __radd__ methods work appropriately
        '''
        if not hasattr(self.lo_class, '__add__'):
            self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="No addition method found")
        elif not hasattr(self.lo_class, '__radd__'):
            self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="No reverse addition method found")

        # test valid types
        for modifier in [get_random_int(1,256),
                       student.IPv4Address(get_random_int(1,256))]:
            with self.subTest(valid_type=type(modifier)):
                new_class = self.lo_class + modifier

                # get the correct type before answer checking
                if isinstance(modifier, student.IPv4Address):
                    modifier = int(repr(modifier))
                new_int = self.lo_int + modifier

                self.custom_test_assert_is_instance(new_class, student.IPv4Address,
                                      Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                      msg="adding involving IP addresses should return an IPv4Address")
                self.custom_test_assert_equal(repr(new_class), str(new_int),
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="adding should allow for int and IPv4Address types")

        # test adding overflow
        with self.subTest("testing overflow of IPv4Address"):
            test_class = student.IPv4Address("255.255.255.255")
            test_class += get_random_int(1,256)
            self.custom_test_assert_equal(str(test_class), "None",
                             Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                             msg="adding overflow should result in 'None' value")

        # test adding None
        with self.subTest("testing adding None"):
            new_class = self.eq_class + None
            self.custom_test_assert_is_instance(new_class, student.IPv4Address,
                                  Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                  msg="adding None to an IP address should return an IPv4Address")
            self.custom_test_assert_equal(id(new_class), id(self.eq_class),
                             Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                             msg="adding None to an IP address should return the original IPv4Address, not a new one")

        # test reverse adding
        with self.subTest("testing reverse adding"):
            modifier = get_random_int(1,256)
            new_class = modifier + self.lo_class
            self.custom_test_assert_is_instance(new_class, student.IPv4Address,
                                  Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                  msg="reverse adding an IPv4Address should return an IPv4Address")

            self.custom_test_assert_equal(repr(new_class), str(self.lo_int + modifier),
                             Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                             msg="reverse adding an IPv4Address should be supported")

        # test adding invalid
        ip_address = [get_random_int(0, 256) for __ in range(4)]
        ip_address = '.'.join(list(map(str, ip_address)))

        self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"])
        with self.assertRaises(student.NotAnIPAddressError,
                               msg="adding an invalid type should throw an error"):
            self.eq_class += ip_address
        print("PASS, ", file=self.out)

    def test_subtraction(self):
        '''
        Test the __sub__ and __rsub__ methods work appropriately
        '''
        if not hasattr(self.lo_class, '__sub__'):
            self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="No subtraction method found")
        elif not hasattr(self.lo_class, '__rsub__'):
            self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="No reverse subtraction method found")

        # test valid types
        for modifier in [get_random_int(1,256),
                       student.IPv4Address(get_random_int(1,256))]:
            with self.subTest(valid_type=type(modifier)):
                new_class = self.hi_class - modifier

                # get the correct type before answer checking
                if isinstance(modifier, student.IPv4Address):
                    modifier = int(repr(modifier))
                new_int = self.hi_int - modifier

                self.custom_test_assert_is_instance(new_class, student.IPv4Address,
                                      Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                      msg="subtracting involving IP addresses should return an IPv4Address")
                self.custom_test_assert_equal(repr(new_class), str(new_int),
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="subtracting should allow for int and IPv4Address types")

        # test subtracting underflow
        with self.subTest("testing underflow of IPv4Address"):
            test_class = student.IPv4Address("0.0.0.0")
            test_class -= get_random_int(1,256)
            self.custom_test_assert_equal(str(test_class), "None",
                             Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                             msg="subtracting underflow should result in 'None' value")

        # test subtracting None
        with self.subTest("testing subtracting None"):
            new_class = self.eq_class - None
            self.custom_test_assert_is_instance(new_class, student.IPv4Address,
                                  Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                  msg="subtracting None to an IP address should return an IPv4Address")
            self.custom_test_assert_equal(id(new_class), id(self.eq_class),
                             Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                             msg="subtracting None to an IP address should return the original IPv4Address, not a new one")

        # test reverse subtracting integer
        with self.subTest("testing reverse subtracting an integer"):
            modifier = get_random_int(4294960000, 4294967296)
            new_class = modifier - self.lo_class
            self.custom_test_assert_is_instance(new_class, student.IPv4Address,
                                  Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                  msg="reverse subtracting should return an IPv4Address")
            self.custom_test_assert_equal(repr(new_class), str(modifier - self.lo_int),
                             Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                             msg="reverse subtracting integers should be supported")

        # test reverse subtracting None
        with self.subTest("testing reverse subtracting None"):
            new_class = None - self.eq_class
            self.custom_test_assert_is_none(new_class,
                             Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                             msg="subtracting an IP address from None should return None")

        # test subtracting invalid
        ip_address = [get_random_int(0, 256) for __ in range(4)]
        ip_address = '.'.join(list(map(str, ip_address)))

        self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"])
        with self.assertRaises(student.NotAnIPAddressError,
                               msg="subtracting an invalid type should throw an error"):
            self.eq_class -= ip_address
        print("PASS, ", file=self.out)

        self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"])
        with self.assertRaises(student.NotAnIPAddressError,
                               msg="Subtracting an IP address from an invalid type should throw an error"):
            new_class = ip_address - self.eq_class
        print("PASS, ", file=self.out)

    def test_equal_to(self):
        '''
        Test the __eq__ method works appropriately
        '''

        # test all valid equal to comparitors
        for option in [self.lo_int,
                       self.lo_str,
                       self.eq_class]:
            with self.subTest(comparitor_type=type(option)):
                self.custom_test_assert_equal(self.lo_class, option,
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="equal comparison should allow int value, string octets, and other IP addresses")

        # test comparing to None
        with self.subTest("testing comparison to None"):
            test_class = student.IPv4Address(-1)
            self.custom_test_assert_equal(test_class, None,
                             Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                             msg="invalid IP addresses should be comparable to None")

        # test error handling
        self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"])
        with self.assertRaises(student.NotAnIPAddressError,
                               msg="invalid comparitors should throw an error"):
            self.eq_class == []
        print("PASS, ", file=self.out)

    def test_not_equal_to(self):
        '''
        Test the __ne__ method works appropriately
        '''

        # test all valid not equal to comparitors
        for option in [self.lo_int,
                       self.lo_str,
                       self.lo_class]:
            with self.subTest(comparitor_type=type(option)):
                self.custom_test_assert_not_equal(self.hi_class, option,
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="not equal comparison should allow int value, string octets, and other IP addresses")

        # test comparing to None
        with self.subTest("testing comparison to None"):
            self.custom_test_assert_not_equal(self.hi_class, None,
                             Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                             msg="invalid IP addresses should be comparable to None")

        # test error handling
        self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"])
        with self.assertRaises(student.NotAnIPAddressError,
                               msg="invalid comparitors should throw an error"):
            self.eq_class != []
        print("PASS, ", file=self.out)

    def test_less_than(self):
        '''
        Test the __lt__ method works appropriately
        '''
        failure = False
        try:
            self.lo_class < 0
        except TypeError:
            failure = True
        if failure:
            self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="No less than function found")

        # test all valid not equal to comparitors
        for option in [self.hi_int,
                       self.hi_str,
                       self.hi_class]:
            with self.subTest(comparitor_type=type(option)):
                self.custom_test_assert_less(self.lo_class, option,
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="less than comparison should allow int value, string octets, and other IP addresses")

        # test error handling
        self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"])
        with self.assertRaises(student.NotAnIPAddressError,
                               msg="invalid IP addresses should thrown an error"):
            self.lo_class < "256.0.0.0"
        print("PASS, ", file=self.out)

        self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"])
        with self.assertRaises(student.NotAnIPAddressError,
                               msg="invalid comparitors should throw an error"):
            self.eq_class < []
        print("PASS, ", file=self.out)

    def test_less_than_or_equal_to(self):
        '''
        Test the __le__ method works appropriately
        '''
        failure = False
        try:
            self.lo_class <= 0
        except TypeError:
            failure = True
        if failure:
            self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="No less than or equal to function found")

        # test all valid not equal to comparitors
        for option in [self.hi_int,
                       self.hi_str,
                       self.hi_class]:
            with self.subTest(comparitor_type=type(option)):
                self.custom_test_assert_less_equal(self.lo_class, option,
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="less than or equal to comparison should allow int value, string octets, and other IP addresses")
                self.custom_test_assert_less_equal(self.lo_class, self.eq_class,
                                     Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                     msg="less than or equal to comparison should allow equal values to compare")

        # test error handling
        self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"])
        with self.assertRaises(student.NotAnIPAddressError,
                               msg="invalid IP addresses should thrown an error"):
            self.lo_class <= "256.0.0.0"
        print("PASS, ", file=self.out)

        self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"])
        with self.assertRaises(student.NotAnIPAddressError,
                               msg="invalid comparitors should throw an error"):
            self.eq_class <= []
        print("PASS, ", file=self.out)

    def test_greater_than(self):
        '''
        Test the __gt__ method works appropriately
        '''
        failure = False
        try:
            self.lo_class > 0
        except TypeError:
            failure = True
        if failure:
            self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="No greater than function found")

        # test all valid not equal to comparitors
        for option in [self.lo_int,
                       self.lo_str,
                       self.lo_class]:
            with self.subTest(comparitor_type=type(option)):
                self.custom_test_assert_greater(self.hi_class, option,
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="greater than comparison should allow int value, string octets, and other IP addresses")

        # test error handling
        self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"])
        with self.assertRaises(student.NotAnIPAddressError,
                               msg="invalid IP addresses should thrown an error"):
            self.lo_class > "256.0.0.0"
        print("PASS, ", file=self.out)

        self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"])
        with self.assertRaises(student.NotAnIPAddressError,
                               msg="invalid comparitors should throw an error"):
            self.eq_class > []
        print("PASS, ", file=self.out)

    def test_greater_than_or_equal_to(self):
        '''
        Test the __ge__ method works appropriately
        '''
        failure = False
        try:
            self.lo_class >= 0
        except TypeError:
            failure = True
        if failure:
            self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="No greater than or equal to function found")

        # test all valid not equal to comparitors
        for option in [self.lo_int,
                       self.lo_str,
                       self.lo_class]:
            with self.subTest(comparitor_type=type(option)):
                self.custom_test_assert_greater_equal(self.hi_class, option,
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="greater than or equal to comparison should allow int value, string octets, and other IP addresses")
                self.custom_test_assert_greater_equal(self.lo_class, self.eq_class,
                                     Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                     msg="greater than or equal to comparison should allow equal values to compare")

        # test error handling
        self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"])
        with self.assertRaises(student.NotAnIPAddressError,
                               msg="invalid IP addresses should thrown an error"):
            self.lo_class >= "256.0.0.0"
        print("PASS, ", file=self.out)

        self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"])
        with self.assertRaises(student.NotAnIPAddressError,
                               msg="invalid comparitors should throw an error"):
            self.eq_class >= []
        print("PASS, ", file=self.out)

# pylint: enable-msg=W0703

# pylint: disable-msg=W0703
class VehicleFactoryTestSuite(CustomUnitTest):
    tag = Suite.Py_OOP

    def setUp(self):
        self.factory = student.VehicleFactory()

    def test_make_function(self):
        '''
        Ensure the 'make' function works appropriately
        '''
        failure = False
        try:
            func = self.factory.make
        except AttributeError:
            failure = True
        if failure:
            self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                            msg="No 'make' function found")

        # test invalid arguments
        for option in [get_random_int(),
                       get_random_str()]:
            with self.subTest(argument=option):
                return_value = self.factory.make(option)
                self.custom_test_assert_equal(return_value, None,
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b"],
                                 msg="invalid arguments should return None")

        # test each vehicle type
        for option, value in [("Car", 5/4),
                              ("Motorcycle", 1/2),
                              ("SemiTruck", 2/18)]:
            with self.subTest(vehicle_type=option):
                # ensure each instance is unique
                vehicle1 = self.factory.make(option)
                vehicle2 = self.factory.make(option)
                self.custom_test_assert_not_equal(id(vehicle1), id(vehicle2),
                                    Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9k"],
                                    msg="each instance created should be unique")

                # attempt to find the vehicle inside the list of Vehicle subclasses
                found = False
                for subclass in student.Vehicle.__subclasses__():
                    if isinstance(vehicle1, subclass):
                        found = True
                        break

                # ensure the instance is a subclass of Vehicle
                if not found:
                    self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9e"],
                                    msg="'{}' does not inherit 'Vehicle'".format(instance))

                # ensure passenger to wheel ratio is correct
                self.custom_test_assert_equal(vehicle1.get_passenger_to_wheel_ratio(), value,
                                 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                 msg="passenger-to-wheel ratio is incorrect")


        # test that the function is a static method
        function = self.factory.make
        static_method_type = types.FunctionType
        self.custom_test_assert_is_instance(function, static_method_type,
                              Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b"],
                              msg="function 'make' is not a static method")

    def test_vehicle_factory_is_singleton(self):
        '''
        Ensure the VehicleFactory is a Singleton
        '''
        factory1 = student.VehicleFactory()
        factory2 = student.VehicleFactory()
        self.custom_test_assert_equal(id(factory1), id(factory2),
                         Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9f", "3.2.9l"],
                         msg="VehicleFactory is not a Singleton")

class WaterCraftAdapterTestSuite(CustomUnitTest):
    tag = Suite.Py_OOP

    def test_WaterCraft_only_allows_Boats(self):
        '''
        Ensure that an error is raised if the WaterCraft isn't a Boat
        '''
        # ensure there are classes that inherit Vehicle to begin with
        subclasses = student.Vehicle.__subclasses__()
        if len(subclasses) <= 1:
            self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9e"],
                            msg="No additional subclasses of 'Vehicle' found")

        # ensure that only Boat, which doesn't inherit Vehicle, can be a WaterCraft
        for non_boat_instance_class in subclasses:
            if non_boat_instance_class == student.WaterCraft:
                continue
            non_boat_instance = non_boat_instance_class()

            self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b"])
            with self.assertRaises(student.NotABoatError,
                                   msg="only 'Boat' can be a WaterCraft"):
                new_watercraft = student.WaterCraft(non_boat_instance)
            print("PASS, ", file=self.out)

    def test_Boat_adapater(self):
        '''
        Ensure 'Boat' can be properly adapted
        '''
        # ensure the class 'Boat' exists
        failure = False
        try:
            boat_obj = student.Boat()
        except NameError:
            failure = True
        if failure:
            self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b"],
                            msg="No 'Boat' class found")

        # ensure there is only one variable stored for Boats
        dictionary_of_instance_variables = vars(boat_obj)
        number_of_instance_variables = len(dictionary_of_instance_variables)
        self.custom_test_assert_equal(number_of_instance_variables, 1,
                         Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9m"],
                         msg="Boat objects should only store one variable")

        # ensure WaterCraft has a function called 'get_passenger_to_wheel_ratio'
        watercraft_obj = student.WaterCraft(boat_obj)
        list_of_WaterCraft_functions = dir(watercraft_obj)
        self.custom_test_assert_in("get_passenger_to_wheel_ratio", list_of_WaterCraft_functions,
                      Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9m"],
                      msg="function 'get_passenger_to_wheel_ratio' not found")

        # ensure there is only one non-dunder method in Boats
        list_of_instance_methods = [item for item in dir(boat_obj)
    								if not item.startswith('__')
    								and not item.endswith('__')
    								and callable(getattr(boat_obj, item))]
        number_of_instance_methods = len(list_of_instance_methods)
        self.custom_test_assert_equal(number_of_instance_methods, 1,
    					 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9m"],
    					 msg="Boat objects should have exactly one method")

        # ensure the one Boat method is the adapter method
        method_name = list_of_instance_methods[0]
        callable_method = getattr(boat_obj, method_name)
        failure = False
        try:
            return_value_from_callable = callable_method()
            self.custom_test_assert_equal(return_value_from_callable, 7,
    						 Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9m"],
    						 msg="Boat method '{}' does not appear to adapt Boats properly".format(method_name))
        except TypeError:
            failure = True
        if failure:
            self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9m"],
    						msg="Boat method '{}' should not have any arguments".format(method_name))

        # ensure the adapter works correctly
        return_value_from_adapter = watercraft_obj.get_passenger_to_wheel_ratio()
        self.custom_test_assert_equal(return_value_from_adapter, 7,
                         Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9m"],
                         msg="adapatation of passenger-to-wheel ratio function incorrect")

        # ensure Boat adapter method is called
        adapter_called = is_called(watercraft_obj.get_passenger_to_wheel_ratio, callable_method)
        self.custom_test_assert_true(adapter_called,
                         Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9m"],
                         msg="Boat method '{}' should be called by WaterCraft method 'get_passenger_to_wheel_ratio'".format(method_name))

class TestVehicleProductionBridger(CustomUnitTest):
    tag = Suite.Py_OOP

    def setUp(self):
        # select a random vehicle to try
        vehicle_instance_class = [i for i in student.Vehicle.__subclasses__() if i != student.WaterCraft]
        if len(vehicle_instance_class) == 0:
            self.custom_test_fail(Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9e"],
                            msg="no classes that inherit 'Vehicle' could be found")
        vehicle_instance_class = random.choice(vehicle_instance_class)
        self.vehicle_instance = vehicle_instance_class()

    def test_invalid_arugments(self):
        '''
        Ensures error raised if arguments are invalid
        '''
        # select a random manufacturer to try
        manufacturer_instance_class = random.choice(student.Manufacturer.__subclasses__())
        manufacturer_instance = manufacturer_instance_class()

        # ensure duplicate instances as parameters throws an error
        for instance in [self.vehicle_instance,
                         manufacturer_instance]:
            with self.subTest(instance_type=type(instance)):

                self.custom_test_print("custom_test_assert_raises", Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9n"])
                with self.assertRaises(student.ArgumentInvalidTypeError,
                                       msg="requires one vehicle and one manufacturer"):
                    bridger = student.VehicleProductionBridger(instance, instance)
                print("PASS, ", file=self.out)

    def test_manufacturers_work_functions_return_values(self):
        '''
        Test that the user created 'work' functions have correct return values
        '''
        manufacturer_instance_class = student.Manufacturer.__subclasses__()
        work_function_return_values = set()

        # check all manufacturers to ensure the return tuples
        for instance_class in manufacturer_instance_class:
            instance = instance_class()
            instance_return_value = instance.work()
            self.custom_test_assert_is_instance(instance_return_value, tuple,
                                  Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                                  msg="return type from 'work' function should be a tuple")
            work_function_return_values.add(instance_return_value)

        # make sure there are only two manufacturers, they don't overlap, and only
        #    allow for one spare
        correct_answer = {(0,1), (1,0)}
        self.custom_test_assert_set_equal(work_function_return_values, correct_answer,
                            Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9j"],
                            msg="each manufacturer should only allow one spare to be returned")

    def test_manufacturer_add_spare(self):
        '''
        Test the add spare function works properly
        '''
        # test both adding spare seats and tires
        for name, instance, function in [("seat", (0,1), self.custom_test_assert_greater),
                                         ("tire", (1,0), self.custom_test_assert_less)]:
            with self.subTest(spare=name):
                # find the manufacturer class the produces each type
                manufacturer_instance_class = student.Manufacturer.__subclasses__()
                manufacturer_instance = None
                for instance_class in manufacturer_instance_class:
                    manufacturer_instance = instance_class()
                    retval = manufacturer_instance.work()
                    if retval == instance:
                        break

                # create the bridger using the manufacturer
                bridger = student.VehicleProductionBridger(self.vehicle_instance, manufacturer_instance)

                # ensure the ratio changes in the appropriate direction for calling the function
                ratio_before_spare = self.vehicle_instance.get_passenger_to_wheel_ratio()
                bridger.add_spare()
                ratio_after_spare = self.vehicle_instance.get_passenger_to_wheel_ratio()
                function(ratio_after_spare, ratio_before_spare,
                         Suite.Py_FUNDAMENTALS, ["3.2.9a", "3.2.9b", "3.2.9n"],
                         msg="ratio between passengers and wheels should change as appropriate")

# pylint: enable-msg=W0703

def suite():
    """Testing"""
    test_suite = unittest.TestSuite()
    test_suite.addTest(VehicleFactoryTestSuite("test_make_function"))
    test_suite.addTest(VehicleFactoryTestSuite("test_vehicle_factory_is_singleton"))
    test_suite.addTest(WaterCraftAdapterTestSuite("test_WaterCraft_only_allows_Boats"))
    test_suite.addTest(WaterCraftAdapterTestSuite("test_Boat_adapater"))
    test_suite.addTest(TestVehicleProductionBridger("test_invalid_arugments"))
    test_suite.addTest(TestVehicleProductionBridger("test_manufacturers_work_functions_return_values"))
    test_suite.addTest(TestVehicleProductionBridger("test_manufacturer_add_spare"))

    test_suite = unittest.TestSuite()
    test_suite.addTest(IPv4AddressTestSuite("test_init_str_and_repr_methods"))
    test_suite.addTest(IPv4AddressTestSuite("test_get_set_ip_from_any_variable_name"))
    test_suite.addTest(IPv4AddressTestSuite("test_is_multicast"))
    test_suite.addTest(IPv4AddressTestSuite("test_is_private"))
    test_suite.addTest(IPv4AddressTestSuite("test_addition"))
    test_suite.addTest(IPv4AddressTestSuite("test_subtraction"))
    test_suite.addTest(IPv4AddressTestSuite("test_equal_to"))
    test_suite.addTest(IPv4AddressTestSuite("test_not_equal_to"))
    test_suite.addTest(IPv4AddressTestSuite("test_less_than"))
    test_suite.addTest(IPv4AddressTestSuite("test_less_than_or_equal_to"))
    test_suite.addTest(IPv4AddressTestSuite("test_greater_than"))
    test_suite.addTest(IPv4AddressTestSuite("test_greater_than_or_equal_to"))
    return test_suite

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