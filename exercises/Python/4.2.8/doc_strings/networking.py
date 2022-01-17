#!/usr/bin/env python3

"""JQR Items Covered

3.2.8 
title: Demonstrate skill in using networking methods and functions, accounting 
for endianness

Imports from standard python libraries are allowed.

"""


def sendTCP(IP, port, buffer):
    """
    Connects to given string `IP` and integer `port`, sends contents from 
    bytes object `buffer`, closes connection after sending. 
    
    Return bytes sent or negative value for errors.
    """


def sendUDP(IP, port, buffer):
    """
    Sends contents of `buffer` to given `IP` and `port`. 
    
    Return bytes sent or negative value for errors.
    """


def converseTCP(port):
    """
    Starts a server on TCP `port`. Testing program connects and a 
    conversation occurs. At some points, the Testing program requests the
    hostname of the server. Testing program terminates conversation. This 
    models client-server interaction.

    Conversation protocol:
    Client sends            Server responds
    int32_t 0x0EA7BEEF      int32_t 0xCAFEBABE
    int32_t 0x90            char[] "NOP"
    int32_t 0xF007          int32_t[] {0x70, 0x33445566, 0xE73BCCDD}
    int32_t 0x1             char[256] {hostname}

    This function will initiate the communication by sending a message and 
    immediately entering recv state to await next action, until client
    termination signal is received.
    """


def converseUDP(port):
    """
    Listens on UDP `port`. Testing program starts a conversation. 
    Occasionally, the hostname is requested. This models client-server 
    interaction.

    Conversation protocol:
    Client sends    Server responds
    Hi.             Hello.
    Nice weather?   Too sunny.
    Ribbit.         Frogs are slimy.
    HOSTNAME        {hostname}
    Bye.            Later. (Conversation terminates)

    This function will initiate the communication by sending a message and 
    immediately entering recv state to await next action, until client
    termination signal is received.
    """
