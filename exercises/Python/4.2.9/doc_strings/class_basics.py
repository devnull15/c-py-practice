#!/usr/bin/env python3
"""
Show the ability to operate with classes
DO NOT import any additional modules
"""


class NotAnIPAddressError(TypeError):
    """Raise this error if the type isn't an IP address or integer"""
    pass


class IPv4Address:
    """
    This class will store an IPv4 address as an integer

    The regular string output should be the IPv4 address in 4-octet format, while
        the alternate/debugging string representation should be the integer (cast
        as a string) value of the IP address.

    The class should be able to add and substract both other IPv4Addresses as well
        as raw integer values. Adding or Subtracting the value None should return
        the original instance. If the type is invalid, raise a NotAnIPAddressError.
        Don't forget to handle addition and subtraction in both ways
        (i.e. IPv4Address + 1 as well as 1 + IPv4Address). Note: None - IPv4Address
        should return None.

    The class should also be able to handle all basic comparisons (i.e. equal to,
        not equal to, less than, less than or equal to, greater than, and greater
        than or equal to). Comparisons should work if the comparison is with an
        integer, a string , or another IPv4Address. Equal and not equal comparisons
        should also be able to compare to None. If comparison is an invalid string or
        any other type not mentioned, raise a NotAnIPAddressError.
    """

    def __init__(self, address):
        """
        Upon initialization, the only parameter should be the IP address to store. IP
        addresses can be either an integer value or a string of 4 octets or integer value.
        If the address is invalid, the stored value should be None. When getting or
        setting the address's value, it should not matter what attribute name is used.
        (See __getattribute__ and __setattr__ methods, as well as the super() function)
        """
        pass

    def is_multicast(self):
        """
        Turn this function into a property that determines if the
        IPv4 Address is multicast, as per RFC 5771

        Return:
            boolean True if the IPv4 address is multicast
            boolean False otherwise
        """
        pass

    def is_private(self):
        """
        This function determines if the IPv4 address is a private address

        Return:
            boolean True if the IPv4 address is private
            boolean False otherwise
        """
        pass
