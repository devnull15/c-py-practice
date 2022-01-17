"""JQR 3.5.4

title: demonstrate the ability to use the following constructs associated with concurrency
sub_items:
  3.5.4.a: threads
  3.5.4.b: locks

"""

import socket
import threading
import time



class Inventory:

    """
    This inventory object is provided for you.
    
    You must use this object to track your bookstore inventory.
    You may not modify the inventory class.

    You can generally use this object like a normal integer,
    however, when updating the inventory you should use the 
    value property. For example:
    my_inventory = Inventory(10)
    my_inventory.value = my_inventory +5

    """

    def __init__(self,initial_value):
        self.value = initial_value


    @property
    def value(self):
        value = self._value
        time.sleep(0.1)
        return value

    @value.setter
    def value(self,new_value):
        self._value = new_value

    def __repr__(self):
        return repr(self.value)

    def __str__(self):
        return str(self.value)

    def __add__(self, other):
        return self.value + other

    def __radd__(self, other):
        return self + other

    def __sub__(self, other):
        return self.value - other

    def __rsub__(self, other):
        return other - self.value

    def __eq__(self, other):
        return self.value == other

    def __ne__(self, other):
        return self.value != other

    def __lt__(self, other):
        return self.value < other

    def __le__(self, other):
        return self.value <= other

    def __gt__(self, other):
        return self.value > other

    def __ge__(self, other):
       return self.value >= other



class Bookstore:

    """
    This is class extends the bookstore in 3.2.6 to add additional methods, 
    a TCP-based server, and support for concurrent connections

    The book store supports buying, selling, and polling the bookstore
    to determine balance of available books.

    The book also supports a method to close the bookstore, ending
    all established sockets and cleanly exiting threads. 

    When the bookstore is created, it will start a listener thread
    on the port provided and wait for new clients to connect.
    When new clients connect, clients should be handled in a separate
    thread. 

    Clients can buy books from the store or sell books to the store.
    These actions occur from the perspective of the customer. 

    The bookstore balance must stay above zero.

    If a customer tries to buy more books than are available,
    the server should respond with, "NotEnoughBooks"

    If a customer tries to do a behavior that is not supported, the server
    should respond with "bad"

    Clients can send the following messages:

    "poll" - display the number of books that the
    store has available. 
      - example:
          client: "poll"
          server: "15"

    "buy <number>" - buy <number> books from the store. 
       - the server should respond with the remaining
       balance in the store
       - example:
            client: "poll"
            server: "15"
            client: "buy 10"
            server: "5"

    "sell <number>" - sell <number> books to the store.
      - the server should respond with the new balance
      in the store
      - example:
            client: "poll"
            server: "5"
            client: "sell 20"
            server: "25"

    "hangup" - when the client sends the hangup message,
    the server should end disconnect from the client. 

    Sessions should stay active until either the store closes
    or a client sends the "hangup" message.

    Students are encouraged to add additional methods to the class to 
    assist in completing the exercise. 

    attributes:
      num_books - the current balances of books. must 

    """


    def __init__(self, port, num_books, host="127.0.0.1"):
        """
        Initialize the bookstore with num_books books.

        @param num_books Number of books. Must not be negative (you should assert this).
        @param host the IP or hostname to listen on; defaults to 127.0.0.1, or localhost 
        @param port the port to listen on; must be greater than 1024 and a valid port (assert this)
        """
 
        self.inventory = Inventory(num_books)


    def buy(self, num_books):
        """
        Buy books from the store.

        If there are not enough books available, respond with "NotEnoughBooks". 
        Customers can buy zero books. If customers try to buy a negative number of books,
        respond with "bad"
        
        Should safely reduce the store balance. 

        @param num_books Number of book to buy. Must not be negative 

        should return a string of the current store balance after the transaction
        
        """


    def sell(self, num_books):
        """
        Sell books to the store

         Customers can sell zero books. If customers try to buy a negative number of books,
        respond with "bad"

        should safely increase the store balance

        @param num_books Number of book to buy. Must not be negative

        should return a string of the current store balance after the transaction
        
        """

    def poll(self):
        """
        poll the store to determine the store balance.

        should safely determine the store balance at time of running

        """


    def is_open(self):
        """
        Tell if the store is closed

        returns a boolean value; true when open, false when closed
        """

    def close_store(self):
        """
        close the store.

        safely close all threads and sockets

        """