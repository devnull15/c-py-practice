"""JQR 3.2.6

title: Demonstrate the ability to perform data validation
sub_items:
  3.2.6.a: Validate that received input matches expected input
  3.2.6.b: Create a method for exception handling
  3.2.6.c: Implement a method for exception handling.
"""

class NotEnoughBooksError(Exception):
    """
    This is a class for custom exception handling.
    Follow standard of other exception classes.
    """
    def __init__(self, message):
    """
    Initialize the booNotEnoughBooksErrorkstore with message.
    
    @param 'message' Custom Error message (String).
    """
    pass

class Bookstore:
    """
    This is a class for a pretty dumb book store. It only sells one book.

    Attributes:
        num_books -- number of books available
    """
    def __init__(self, num_books):
        """
        Initialize the bookstore with num_books books.

        @param num_books Number of books. Must not be negative (you should assert this).
        """
        pass

    def buy(self, num_books):
        """
        Buy a book.

        If there are not enough books available, raise NotEnoughBooksError.

        @param num_books Number of book to buy. Must not be negative (you should assert this)
        (Buying zero book is allowed.)
        """
        pass