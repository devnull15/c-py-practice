class NotEnoughBooksError(Exception):
    """
    This is a class for custom exception handling.
    Follow standard of other exception classes.
    """

    """
    Initialize the booNotEnoughBooksErrorkstore with message.
    
    @param 'message' Custom Error message (String).
    """
    
    def __init__(self, message):
        self.message = message
        super().__init__(self.message)

class Bookstore:
    """
    This is a class for a pretty dumb book store. It only sells one book.

    Attributes:
        num_books -- number of books available
    """
    num_books = 0
    
    def __init__(self, num_books):
        """
        Initialize the bookstore with num_books books.

        @param num_books Number of books. Must not be negative (you should assert this).
        """
        assert num_books >= 0, NotEnoughBooksError("Books can't be negative!")
        self.num_books = num_books

    def buy(self, num_books):
        """
        Buy a book.

        If there are not enough books available, raise NotEnoughBooksError.

        @param num_books Number of book to buy. Must not be negative (you should assert this)
        (Buying zero book is allowed.)
        """
        assert num_books >= 0, NotEnoughBooksError("Books can't be negative!")
        if self.num_books < num_books:
            raise NotEnoughBooksError("Not enough books!")
        else:
            self.num_books -= num_books
