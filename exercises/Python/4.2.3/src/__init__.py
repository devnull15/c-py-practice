def add_me(x, y):
    """Addition

    Write a function that adds 2 inputs and returns the answer
    """
    return x+y


def subtract_me(x, y):
    """Subtraction

    Write a function that subtracts the second input from the first input and returns the answer
    """
    return x-y


def multiply_me(x, y):
    """Mulitplication

    Write a function that multiplies two inputs and returns the answer
    """
    return x*y


def divide_me(x, y):
    """Division

    Write a function that divides the first input by the second input and returns the answer.
    If division by zero is attempted, return None.
    """
    if y==0: return None
    return x/y


def mod_me(x, y):
    """Modulo

    Write a function that takes the modulus of the
    first input by the second input and returns the answer
    """
    return x%y
