class NotAnIPAddressError(TypeError):
    """Raise this error if the type isn't an IP address or integer"""
    def __init__(self, addr, message="! Invalid IPv4 Address /"):
        self.message = message
        self.message += "/ IP: %s /" % str(addr)
        self.message += "/ Type: %s /" % type(addr)
        super().__init__(self.message)


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

    FIRSTOCTETMASK = 4278190080 # 255.0.0.0
    MAX_IP_INT = 4294967295 # 255.255.255.255
    MIN_IP_INT = 0 # 0.0.0.0
    
    def _int_to_str(self):
        address = self.ip
        ret = ""
        mask = self.FIRSTOCTETMASK
        for i in range(3,-1,-1):
            octet = address & mask
            octet = octet // (256**i)
            ret += str(octet)
            if(i > 0):
                ret += "."
            mask = mask // 256
        return ret

    
    def _str_to_int(self, address):
        ipList = address.split('.')
        if(len(ipList) != 4):
            raise NotAnIPAddressError(address)
        count = 3
        ipInt = 0
        for octet in ipList:
            if(not octet.isdigit()):
                raise NotAnIPAddressError(address)                
            i = int(octet)
            if(i < 0 or i > 255):
                raise NotAnIPAddressError(address)
            ipInt += i*(256**count)
            count -= 1
        return ipInt

    def _parse_address(self, address):
        if(isinstance(address,int) and self.MIN_IP_INT <= address <= self.MAX_IP_INT):
            ip = address
        elif(isinstance(address,str) and '.' not in address):
            if(self.MIN_IP_INT <= int(address) <= self.MAX_IP_INT):
                ip = int(address)
            else:
                raise NotAnIPAddressError(address)
        elif(isinstance(address,str)):
            ip = self._str_to_int(address)
        else:
            raise NotAnIPAddressError(address)
        return ip
        
    
    def __init__(self, address):
        """
        Upon initialization, the only parameter should be the IP address to store. IP
        addresses can be either an integer value or a string of 4 octets or integer value.
        If the address is invalid, the stored value should be None. When getting or
        setting the address's value, it should not matter what attribute name is used.
        (See __getattribute__ and __setattr__ methods, as well as the super() function)
        """
        try:
            self.ip = self._parse_address(address)
        except NotAnIPAddressError:
            self.ip = None

        
    def __getattr__(self, attr):
        return self.ip

    def __setattr__(self, key, value):
        try:
            ip = self._parse_address(value)
        except NotAnIPAddressError:
            ip = None
        finally:
            super().__setattr__("ip", ip)
    
    def __str__(self):
        if(self.ip is None):
            return "None"
        return self._int_to_str()

    def __repr__(self):
        return str(self.ip)

    
    def __add__(self,address):
        if(isinstance(address,int)):
            new_addr = self.ip + address
        elif(isinstance(address,IPv4Address)):
            new_addr = self.ip + address.ip
        elif(address is None):
            return self
        else:
           raise NotAnIPAddressError(address)
        return IPv4Address(new_addr)

    def __radd__(self,other):
        return self.__add__(other)

    def __sub__(self,address):
        if(isinstance(address,int)):
            new_addr = self.ip - address
        elif(isinstance(address,IPv4Address)):
            new_addr = self.ip - address.ip
        elif(address is None):
            return self
        else:
           raise NotAnIPAddressError(address)
        return IPv4Address(new_addr)

    def __rsub__(self,other):
        print("subbing: " + str(other) + ' - ' + str(self))
        if(isinstance(other,int)):
            new_addr = other - self.ip
        elif(isinstance(other,IPv4Address)):
            new_addr = other - self.ip
        elif(other is None):
            return None
        else:
           raise NotAnIPAddressError(other)
        return IPv4Address(new_addr)
    
    def __eq__(self, other):
        if(isinstance(other,int) or other is None):
            if(self.ip == other):
                return True
            else:
                return False
        elif(isinstance(other,IPv4Address)):
            if(self.ip == other.ip):
                return True
            else:
                return False
        elif(isinstance(other,str)):
            if(self.ip == self._str_to_int(other)):
                return True
            else:
                return False
        else:
            raise NotAnIPAddressError(other)

    def __ne__(self, other):
        if(isinstance(other,int) or other is None):
            if(self.ip != other):
                return True
            else:
                return False
        elif(isinstance(other,IPv4Address)):
            if(self.ip != other.ip):
                return True
            else:
                return False
        elif(isinstance(other,str)):
            if(self.ip != self._str_to_int(other)):
                return True
            else:
                return False
        else:
            raise NotAnIPAddressError(other)

    def __gt__(self, other):
        if(isinstance(other,int) or other is None):
            if(self.ip > other):
                return True
            else:
                return False
        elif(isinstance(other,IPv4Address)):
            if(self.ip > other.ip):
                return True
            else:
                return False
        elif(isinstance(other,str)):
            if(self.ip > self._str_to_int(other)):
                return True
            else:
                return False
        else:
            raise NotAnIPAddressError(other)

    def __ge__(self, other):
        if(isinstance(other,int) or other is None):
            if(self.ip >= other):
                return True
            else:
                return False
        elif(isinstance(other,IPv4Address)):
            if(self.ip >= other.ip):
                return True
            else:
                return False
        elif(isinstance(other,str)):
            if(self.ip >= self._str_to_int(other)):
                return True
            else:
                return False
        else:
            raise NotAnIPAddressError(other)

    def __lt__(self, other):
        if(isinstance(other,int) or other is None):
            if(self.ip < other):
                return True
            else:
                return False
        elif(isinstance(other,IPv4Address)):
            if(self.ip < other.ip):
                return True
            else:
                return False
        elif(isinstance(other,str)):
            if(self.ip < self._str_to_int(other)):
                return True
            else:
                return False
        else:
            raise NotAnIPAddressError(other)

    def __le__(self, other):
        if(isinstance(other,int) or other is None):
            if(self.ip <= other):
                return True
            else:
                return False
        elif(isinstance(other,IPv4Address)):
            if(self.ip <= other.ip):
                return True
            else:
                return False
        elif(isinstance(other,str)):
            if(self.ip <= self._str_to_int(other)):
                return True
            else:
                return False
        else:
            raise NotAnIPAddressError(other)

        
    def is_multicast(self):
        """
        Turn this function into a property that determines if the
        IPv4 Address is multicast, as per RFC 5771

        Return:
            boolean True if the IPv4 address is multicast
            boolean False otherwise
        """
        MULTICAST_MIN = 3758096384 # 224.0.0.0
        MULTICAST_MAX = 4026531839 # 239.255.255.255

        
        if(self.ip is None):
            return False
        if(MULTICAST_MIN <= self.ip <= MULTICAST_MAX):
            return True
        else:
            return False
        
    is_multicast = property(is_multicast)
        


    def is_private(self):
        """
        This function determines if the IPv4 address is a private address

        Return:
            boolean True if the IPv4 address is private
            boolean False otherwise
        """
        CLASS_A_MIN = 167772160 # 10.0.0.0
        CLASS_A_MAX = 184549375 # 10.255.255.255
        CLASS_B_MIN = 2886729728 # 172.16.0.0
        CLASS_B_MAX = 2887778303  # 172.31.255.255
        CLASS_C_MIN = 3232235520 # 192.168.0.0
        CLASS_C_MAX = 3232301055 # 192.168.255.255
        LOOPBACK_MIN = 2130706432 # 127.0.0.0
        LOOPBACK_MAX = 2147483647 # 127.255.255.255

        if(self.ip is None):
            return False
        if(CLASS_A_MIN <= self.ip <= CLASS_A_MAX or
           CLASS_B_MIN <= self.ip <= CLASS_B_MAX or
           CLASS_C_MIN <= self.ip <= CLASS_C_MAX or
           LOOPBACK_MIN <= self.ip <= LOOPBACK_MAX):
            return True
        else:
            return False


########################################################################



class NotABoatError(TypeError):
    """Raise this error if a watercraft isn't given a Boat"""
    def __init__(self, obj, message=""):
        self.message = "! {0} is Not a boat".format(obj)
        super().__init__(self.message)


class ArgumentInvalidTypeError(TypeError):
    """Raised when an argument is a valid type"""
    def __init__(self, arg, message=""):
        self.message = "! {0} is an invalid argument".format(arg)
        super().__init__(self.message)



class Vehicle:
    """
    You need to make 3 classes that inherit this class
    You should be able to handle the following:
        car: 4 wheels, 5 passengers
        motorcycle: 2 wheels, 1 passenger
        semitruck: 18 wheels, 2 passengers
    """
    def __init__(self, wheels, passengers):
        self.wheels = wheels
        self.passengers = passengers
    

    def get_passenger_to_wheel_ratio(self):
        """
        This method doesn't need to be modified, but you'll need to
        implement code that can use the attributes called in this method
        """
        return self.passengers / self.wheels  #pylint: disable=no-member


class Car(Vehicle):
    def __init__(self):
        super().__init__(4,5)

class Motorcycle(Vehicle):
    def __init__(self):
        super().__init__(2,1)

class Semitruck(Vehicle):
    def __init__(self):
        super().__init__(18,2)



class VehicleFactory:
    """
    VehicleFactory should be implemented as a Singleton. There should only be
7    one actual instance of the VehicleFactory. Hint: Utilize the __new__ method
    to make the class a Singleton.

    You need to implement a static method called 'make' which has one string
    argument of the vehicle to make. Calling this function should return a new
    instance of whatever vehicle is called.

    The possible arguments are "Car", "Motorcycle", and "SemiTruck", and they
    should return their respective instances. Any other input value or type should
    return None.
    """

    __instance = None
    
    def __new__(cls):
        if(VehicleFactory.__instance is None):
            VehicleFactory.__instance = object.__new__(cls)
        return VehicleFactory.__instance

    @staticmethod
    def make(whattomake):
        if(whattomake == "Car"):
            return Car()
        elif(whattomake == "Motorcycle"):
            return Motorcycle()
        elif(whattomake == "SemiTruck"):
            return Semitruck()
        else:
            return None

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
    def __init__(self, cls):
        if(not isinstance(cls,Boat)):
            raise NotABoatError(cls)
        self.boat = cls
        
    def get_passenger_to_wheel_ratio(self):
        return self.boat.get_passenger_to_wheel_ratio()
    

class Boat:
    def __init__(self):
        self.passengers = 7

    def get_passenger_to_wheel_ratio(self):
        return self.passengers
    

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

class SpareTire(Manufacturer):
    def work(self):
        return (1,0)

class SpareSeat(Manufacturer):
    def work(self):
        return (0,1)


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
        if(not isinstance(my_vehicle,Vehicle)):
            raise ArgumentInvalidTypeError(my_vehicle)
        else:
            self.my_vehicle = my_vehicle

        if(not isinstance(my_manufacturer,Manufacturer)):
            raise ArgumentInvalidTypeError(my_manufacturer)
        else:
            self.my_manufacturer = my_manufacturer

        

    def add_spare(self):
        """
        This function should call the manufacturer's 'work' function and add
            the appropriate additional parts
        """
        TIRE = (1,0)
        SEAT = (0,1)
        if self.my_manufacturer.work() == TIRE:
            self.my_vehicle.wheels += 1
        else:
            self.my_vehicle.passengers += 1
