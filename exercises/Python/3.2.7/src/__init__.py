def forLoop(l1):
    """
    Write a function that squares each value in a list.
    Example:
    Input: [2, 4, 6, 8]
    Output: [4, 16, 36, 64]
    """
    ret = []
    for i in l1:
        ret.append(i**2)
    return ret


def whileLoop(l2):
    """
    Write a function that counts down to 5 by taking the input
    and appending the countdown to a list.  Return the list.
    Example:
    Input: 10
    Output: [10, 9, 8, 7, 6, 5]
    """
    ret = []
    while l2 >= 5:
        ret.append(l2)
        l2 -= 1
    return ret


def ifStatement(l3):
    """
    Write a function that takes in a list of numbers and removes any values
    that are divisible by 3
    Example:
    Input: [1, 2, 3, 4]
    Output: [1, 2, 4]
    """
    ret = []
    for i in l3:
        if i%3 != 0:
            ret.append(i)

    return ret


def ifElseStatement(num):
    """
    A prime number is a natural number greater than 1 that is not a product
    of two smaller natural numbers. Write a function that returns True if the
    input is a prime number and False otherwise.
    Example:
    Input: 5
    Result: True
    """
    if num <= 1:
        return False
    else:
        for i in range(2,num-1):
            if num%i == 0:
                return False
    return True


def ifElifElseStatement(test_string):
    """Write a function that accepts a string and returns the number of
    digits and letters in a list, where the first entry is digits, and
    the second entry is letters.
    Example:
    Input: abcd12
    Result: [2, 4]
    """
    ret = [0,0]
    for c in test_string:
        if c.isdigit():
            ret[0]+=1
        elif c.isalpha():
            ret[1]+=1
        else:
            pass
    return ret
