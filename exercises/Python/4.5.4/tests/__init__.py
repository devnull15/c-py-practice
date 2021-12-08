#!/usr/bin/env python3

import unittest
import random
import pathlib
import time
import junit_xml
import importlib.util
import os
import sys
import threading

import os
import random
import socket


#loads library from environmental variable or from a relative path
#https://docs.python.org/3/library/importlib.html#importing-a-source-file-directly

TEST_FILE = os.environ.get("TEST_FILE")

TEST_SUITE_OUTPUT = "suite_results.xml"
TEST_SUITE_NAME = "3.5.4"

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


def start_bookstore(port,books,failing=True):
    bookstore = student.Bookstore(port,books)
    if failing:
        bookstore.close_store()
    return bookstore

def stop_bookstore(store):
    store.close_store()


class Testing(unittest.TestCase):

    def seed_random(self):
        random.seed(os.urandom(128))

    def test_bookstore_negative_port(self):
        try:
            port = -1
            start_bookstore(port,10)
        except AssertionError:
            pass
        else:
            print("Should not be able to create bookstore w/ negative port")
            self.fail()

    def test_low_bookstore_port(self):
        try:
            port = random.randint(500,999)
            start_bookstore(port,10)
            self.fail()
        except Exception as exc:
            pass
    
    def test_high_bookstore_port(self):
        try:
            port = random.randint(65536,100000)
            start_bookstore(port,10)
            self.fail()
        except Exception as exc:
            pass

    def test_bookstore_valid_port(self):
        try:
            port = random.randint(1025,65534)
            bookstore = start_bookstore(port,10,failing=False)
        except Exception as exc:
            print(exc)
            self.fail("failed to create bookstore with valid port: {}".format(exc))
        bookstore.close_store()
        self.assertFalse(bookstore.is_open())


    def test_bookstore_zero_port(self):
        try:
            start_bookstore(0,10)
            print("Port cannot be zero")
            self.fail()
        except AssertionError:
            pass

    def test_bookstore_negative_books(self):
        try:
            port = random.randint(1000,2000)
            start_bookstore(port,-11)
        except AssertionError:
            pass
        else:
            print("Should not be able to create bookstore w/ negative books")
            self.fail()


    def test_buy_negative(self):
        port = random.randint(1025,2000)
        store = start_bookstore(port,random.randint(10,20),failing=False)
        data = store.buy(-10)
        stop_bookstore(store)
        self.assertEqual(data,"bad")
        if data != "bad":
            print("Should not be able to buy negative books")

    def test_buy_zero(self):
        port = random.randint(2000,3000)
        start_books = random.randint(10, 20)
        store = start_bookstore(port,start_books,failing=False)
        data = store.buy(0)
        stop_bookstore(store)
        self.assertEqual(int(data),start_books)

        if int(data) != start_books:
            print("Buying zero books is allowed")

    def test_buy(self):
        port = random.randint(2000,3000)
        start_books = random.randint(10, 20)
        store = start_bookstore(port,start_books,failing=False)
        buy = start_books - 5
        data = store.buy(buy)
        stop_bookstore(store)
        self.assertEqual(int(data),start_books - buy)

    def test_not_enough_books(self):
        start_books = random.randint(10, 20)
        port = random.randint(3000,4000)
        store = start_bookstore(port,start_books,failing=False)
        buy = start_books +1

        resp = store.buy(buy)

        stop_bookstore(store)

        self.assertEqual(resp,"NotEnoughBooks")

        if resp != "NotEnoughBooks":
            print("When there is not enough books, you should respond: 'NotEnoughBooks'")

    def test_sell_negative(self):
        port = random.randint(4000,5000)
        start_books = random.randint(10,20)
        store = start_bookstore(port,start_books,failing=False)

        data = store.sell(-10)

        stop_bookstore(store)

        time.sleep(0.5)


        if data == "bad":
            pass
        else:
            print("Should not be able to sell negative books")
            self.fail()

    def test_sell_zero(self):
        port = random.randint(6000,7000)
        start_books = random.randint(10,20)
        store = start_bookstore(port,start_books,failing=False)
        data = store.sell(0)
        self.assertEqual(int(data),start_books)

        stop_bookstore(store)

        if int(data) != start_books:
            print("Selling 0 books should be allowed")

    def test_sell(self):
        port = random.randint(7000,8000)
        start_books = random.randint(10, 20)
        store = start_bookstore(port,start_books,failing=False)
        sell_amount = 100
        data = store.sell(sell_amount)

        stop_bookstore(store)

        self.assertEqual(int(data),start_books + sell_amount)

    def test_poll(self):
        port = random.randint(8000,9000)
        start_books = random.randint(10, 20)
        store = start_bookstore(port,start_books,failing=False)
        poll_value = int(store.poll())

        stop_bookstore(store)

        self.assertEqual(poll_value,start_books)

        

    def test_race_condition(self):

        port = random.randint(10000,11000)
        start_books = 0
        store = start_bookstore(port,start_books,failing=False)

        def buy_from_store(store):
            time.sleep(2)
            for i in range(3):
                store.sell(random.randint(1,10))
                for j in range(2):
                    try:
                        resp = int(store.buy(random.randint(1,10)))
                        self.assertGreater( resp  ,  -1)
                    except ValueError:
                        pass
                    


        threads = []

        for i in range(10): # create
            threads.append(threading.Thread(target=buy_from_store,args=(store,)))

        for thread in threads: #start all at once
            thread.start()

        for thread in threads: # wait for completion
            thread.join()

        store_value = int(store.poll())

        stop_bookstore(store)

        self.assertGreater( store_value ,  -1)

        



    def test_as_client(self):

        port = random.randint(10000,11000)
        start_books = random.randint(10, 20)
        store = start_bookstore(port,start_books,failing=False)

        socket_list = []

        def converse(start_books,store,do_asserts=False):
            
            host = '127.0.0.1'
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            socket_list.append(sock)
            time.sleep(0.5)
            try:
                sock.connect((host,port))
            except ConnectionRefusedError:
                sock.close()
                self.fail("failed to connect to server")
            
            sock.sendall("poll".encode("utf-8"))
            resp = sock.recv(1024).decode("utf-8")
            if do_asserts:
                self.assertEqual(start_books,int(resp))

            buy = start_books -1
            sock.sendall("buy {}".format(buy).encode("utf-8"))
            resp = sock.recv(1024).decode("utf-8")
            if do_asserts:
                self.assertEqual(int(resp),start_books - buy)

            buy = start_books + 1
            sock.sendall("buy {}".format(buy).encode("utf-8"))
            resp = sock.recv(1024).decode("utf-8")
            if do_asserts:
                self.assertEqual(resp,"NotEnoughBooks")

            sell = start_books
            current = int(store.poll())
            sock.sendall("sell {}".format(sell).encode("utf-8"))
            resp = sock.recv(1024).decode("utf-8")
            if do_asserts:
                self.assertEqual(int(resp),current + sell)

            sock.sendall("hangup".encode("utf-8"))
            sock.close()
               

        def shutdown_sockets():
            runtime = 3
            global MAX_RUNTIME
            time.sleep(runtime)
            while runtime < MAX_RUNTIME:
                time.sleep(1)
                runtime += 1
            for sock in socket_list:
                sock.shutdown(socket.SHUT_RDWR)
                sock.close()

        converse(start_books,store,do_asserts=True)

        threads = []

        

        for i in range(5):
            start_books = int(store.poll())
            thread = threading.Thread(target=converse,args=(start_books,store,False))
            thread.start()
            threads.append(thread)
        
        for thread in threads:
            thread.join()


        stop_bookstore(store)






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
