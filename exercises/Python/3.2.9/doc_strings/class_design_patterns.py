#!/usr/bin/env python3
"""
Show the ability to operate with class design patterns
DO NOT import any additional modules
"""
#pylint: disable-msg=too-few-public-methods


class NotABoatError(TypeError):
    """Raise this error if a watercraft isn't given a Boat"""
    pass


class ArgumentInvalidTypeError(TypeError):
    """Raised when an argument is a valid type"""
    pass


class Vehicle:
    """
    You need to make 3 classes that inherit this class
    You should be able to handle the following:
        car: 4 wheels, 5 passengers
        motorcycle: 2 wheels, 1 passenger
        semitruck: 18 wheels, 2 passengers
    """

    def get_passenger_to_wheel_ratio(self):
        """
        This method doesn't need to be modified, but you'll need to
        implement code that can use the attributes called in this method
        """
        return self.passengers / self.wheels  #pylint: disable=no-member


class VehicleFactory:
    """
    VehicleFactory should be implemented as a Singleton. There should only be
    one actual instance of the VehicleFactory. Hint: Utilize the __new__ method
    to make the class a Singleton.

    You need to implement a static method called 'make' which has one string
    argument of the vehicle to make. Calling this function should return a new
    instance of whatever vehicle is called.

    The possible arguments are "Car", "Motorcycle", and "SemiTruck", and they
    should return their respective instances. Any other input value or type should
    return None.
    """
    pass


class WaterCraft(Vehicle):
    """
    This is an adapter class that will convert and allow water vehicles to be represented

    You should create an __init__ function that takes an instance of a 'Boat' class
        as it's parameter. If the argument isn't a 'Boat' instance, then throw a
        NotABoatError.

    This class should have a function called "get_passenger_to_wheel_ratio"
        (same as Vehicle), but this should call the specific function created in
        the 'Boat' class to compare Boats with other Vehicles.

    The 'Boat' object shouldn't store any attributes other than the number of passengers,
        which will be set at 7, and should have a function to return the number of
        passengers, which should be the function called by the "get_passenger_to_wheel_ratio"
        function in this class.
    """
    pass


class Manufacturer:
    """
    This class will add an additional 'work' function which adds parts to an
        existing Vehicle

    You should create two classes that inherit this one, one that adds a spare seat,
        and one that adds a spare tire. See the notes on the 'work' function for
        details on implementation.
    """

    def work(self):  #pylint: disable=no-self-use
        """
        This function will be overwritten by each sub-class. Each sub class will only
            affect one of the two return values, and should only have an additional
            value of 1.

        Return (tuple):
            Value 1: the number of additional tires to add (max 1)
            Value 2: the number of additional seats to add (max 1)
            NOTE: no Manufacturer can add both a spare seat and a spare tire
        """
        return (0, 0)


class VehicleProductionBridger:
    """
    This is a bridge class that combines a vehicle with a manufacturer
    """

    def __init__(self, my_vehicle, my_manufacturer):
        """
        This function should store the vehicle and manufacturer combination

        Arguments:
            my_vehicle is a Vehicle object to be stored, raise an ArgumentInvalidTypeError
                if the type is incorrect
            my_manufacturer is a Manufacturer object to be stored, raise an
                ArgumentInvalidTypeError if the type is incorrect
        """
        pass

    def add_spare(self):
        """
        This function should call the manufacturer's 'work' function and add
            the appropriate additional parts
        """
        pass
