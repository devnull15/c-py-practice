#!/usr/bin/env python3

import unittest
import random
import pathlib
import importlib.util 
import os
import junit_xml
import sys

import socket
import struct
import random
import time
import threading


#loads library from environmental variable or from a relative path
#https://docs.python.org/3/library/importlib.html#importing-a-source-file-directly

TEST_FILE = os.environ.get("TEST_FILE")

TEST_SUITE_OUTPUT = "suite_results.xml"
TEST_SUITE_NAME = "3.2.8"

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
    """Testing"""
    jqr_items = ""
    extra_tag = ""

    def real_seed(self):
        """Testing"""
        random.seed(os.urandom(128))

    def test_send_tcp(self):
        """
        sendTCP(str IP, int port, bytes buffer)
        Connects to given string `IP` and integer `port`, sends contents from
        bytes object `buffer`, closes connection after sending.

        Return bytes sent or negative value for errors.
        """
        print("\tTesting: sendTCP")
        try:
            the_port = random.randint(32768, 61000)
            the_payload = b"This is a string for test_send_tcp()."
            received_payload = b""

            listening_socket = socket.socket(socket.AF_INET,
                                             socket.SOCK_STREAM, 0)
            listening_socket.bind(("", the_port))
            listening_socket.listen()

            func_test_thread = threading.Thread(
                target=student.sendTCP,
                args=("127.0.0.1", the_port, the_payload))
            func_test_thread.start()

            conn_socket = listening_socket.accept()[0]
            received_payload = conn_socket.recv(128)
            conn_socket.close()
            listening_socket.close()
            func_test_thread.join()
            # print(f"Received {received_payload}")

            self.assertEqual(the_payload, received_payload)
        except Exception as exc:
            print(exc)
            self.fail()
        return

    def test_send_udp(self):
        """
        sendUDP(IP, port, buffer)
        Sends contents of `buffer` to given `IP` and `port`.

        Return bytes sent or negative value for errors.
        """
        print("\tTesting: sendUDP")
        try:
            the_port = random.randint(32768, 61000)
            the_payload = b"This is a string for test_send_udp()."
            received_payload = b""

            the_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
            the_socket.bind(("", the_port))

            func_test_thread = threading.Thread(
                target=student.sendUDP,
                args=("127.0.0.1", the_port, the_payload))
            func_test_thread.start()

            received_payload, sender_info = the_socket.recvfrom(128, 0)
            the_socket.close()
            func_test_thread.join()
            # print(f"Received {received_payload} from {sender_info}.")

            self.assertEqual(the_payload, received_payload)
        except Exception as exc:
            print(exc)
            self.fail()
        return

    def conv_to_bytes(self, in_obj):
        """Testing"""
        in_obj_type = type(in_obj)
        return_thing = b"BLEH"
        if in_obj_type == type("STRING"):
            return_thing = bytes(in_obj, "UTF-8")
        elif in_obj_type == type(0x90):
            return_thing = struct.pack("!L", in_obj)
        elif in_obj_type == type((5, 34)):
            return_thing = struct.pack("!LLL", *in_obj)
        elif in_obj_type == type(b"BYTES"):
            return_thing = in_obj
        else:
            return_thing = bytes(in_obj, "UTF-8")
        return return_thing

    def conv_from_bytes(self, in_bytes):
        """Testing"""
        the_length = len(in_bytes)
        return_thing = ""
        if type(in_bytes) != type(b"BYTES"):
            return in_bytes
        if the_length == 3:
            return_thing = in_bytes.decode()
        elif the_length == 4:
            return_thing = struct.unpack("!L", in_bytes)[0]
        elif the_length == 12:
            return_thing = struct.unpack("!LLL", in_bytes)
        else:
            return_thing = in_bytes.decode()
        return return_thing

    def test_converse_tcp(self):
        """
        Starts a server on TCP `port`.
        Testing program connects and a conversation occurs.
        At some point, the Testing program requests the hostname of the server.
        Testing program terminates conversation.
        This models client-server interaction.
        """
        print("\tTesting: Converse TCP")
        try:
            self.real_seed()
            the_port = random.randint(32768, 61000)
            received_payload = b""
            num_exchanges = 2000
            random.seed(1337)
            the_host_name = socket.gethostname()

            responses = {
                0xCAFEBABE: 0xEA7BEEF,
                "NOP": 0x90,
                (0x00000070, 0x33445566, 0xE73BCCDD): 0xF007,
                the_host_name: 0x1
            }

            send_choices = list(responses.values())

            func_test_thread = threading.Thread(
                target=student.converseTCP, args=(the_port,))
            func_test_thread.start()
            time.sleep(.1)
            the_socket = socket.create_connection(("127.0.0.1", the_port))

            good_count = 0
            bad_count = 0

            for iteration in range(num_exchanges):
                send_thing = random.choice(send_choices)
                the_socket.send(self.conv_to_bytes(send_thing))
                received_payload = the_socket.recv(256)
                converted_payload = self.conv_from_bytes(received_payload)
                # print(f"Got {converted_payload} in response to
                # {send_thing}.")
                if send_thing == responses[converted_payload]:
                    # print("Good.")
                    good_count += 1
                else:
                    # print("Not good.")
                    bad_count += 1

            the_socket.close()
            func_test_thread.join()
            # print(f"Received {good_count} good responses,
            # {bad_count} bad responses out of {num_exchanges} total.")

            self.assertEqual(good_count, num_exchanges)
        except Exception as e:
            print(e)
            self.fail()
        return

    def test_converse_udp(self):
        """
        Listens on UDP `port`. Testing program starts a conversation.
        At one point, the hostname is requested.
        This models client-server interaction.
        """
        print("\tTesting: Converse UDP")
        try:
            self.real_seed()
            the_port = random.randint(32768, 61000)
            received_payload = b""
            num_exchanges = 2000
            random.seed(1337)
            the_host_name = socket.gethostname()

            responses = {
                b"Hello.": b"Hi.",
                b"Too sunny.": b"Nice weather?",
                b"Frogs are slimy.": b"Ribbit.",
                bytes(the_host_name, "UTF-8"): b"HOSTNAME",
            }

            terminator = b"Bye."
            terminator_answer = b"Later."

            send_choices = list(responses.values())

            func_test_thread = threading.Thread(
                target=student.converseUDP, args=(the_port,))
            func_test_thread.start()
            time.sleep(.1)
            address = ("127.0.0.1", the_port)
            the_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)

            good_count = 0
            bad_count = 0

            recv_from_junk = (1, 2)

            for _ in range(num_exchanges-1):
                send_thing = random.choice(send_choices)
                the_socket.sendto(send_thing, 0, address)
                received_payload, recv_from_junk = the_socket.recvfrom(256)
                # print(f"Got {received_payload} in response to {send_thing}.")
                if send_thing == responses[received_payload]:
                    # print("Good.")
                    good_count += 1
                else:
                    # print("Not good.")
                    bad_count += 1

            send_thing = terminator
            the_socket.sendto(send_thing, 0, address)
            received_payload, recv_from_junk = the_socket.recvfrom(256)
            # print(f"Got {received_payload} in response to {send_thing}.")
            if received_payload == terminator_answer:
                good_count += 1
            else:
                bad_count += 1

            the_socket.close()
            func_test_thread.join()
            # print(f"Received {good_count} good responses,
            # {bad_count} bad responses out of {num_exchanges} total.")

            self.assertEqual(good_count, num_exchanges)
        except Exception as e:
            print(e)
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