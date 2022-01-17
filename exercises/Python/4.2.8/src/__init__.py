import socket
import struct

MAX_TCP_MSG = 4
MAX_UDP_MSG = 256

def sendTCP(IP, port, buffer):
    """
    Connects to given string `IP` and integer `port`, sends contents from 
    bytes object `buffer`, closes connection after sending. 
    
    Return bytes sent or negative value for errors.
    """
    ret = 0
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((IP,port))
        ret = s.send(buffer)
    except(socket.error, exc):
        print("Caught socket error: %s" % exc)
        ret = -1
    s.close()
    return ret    

def sendUDP(IP, port, buffer):
    """
    Sends contents of `buffer` to given `IP` and `port`. 
    
    Return bytes sent or negative value for errors.
    """
    ret = 0
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        ret = s.sendto(buffer, (IP,port))
    except(socket.error, exc):
        print("Caught socket error: %s" % exc)
        ret = -1
    s.close()
    return ret    

def _print_msg(msg):
    #x = ''.join(format(x,'02x') for x in msg)
    if(type(msg) == type(b'BYTES')):
       print(msg)
    else:
       x = '{:08x}'.format(msg)
       print(x)
    return 

def _convTCPresp(msg):
    ret = b''
    if(msg==0xf007):
        ret = [0x70, 0x33445566, 0xE73BCCDD]
        ret = struct.pack('!%sL' % len(ret),  *ret)
    elif(msg==0x90):
        ret = b'NOP'
        ret = struct.pack('!%ss' % len(ret),ret)
    elif(msg==0x1): # BUG: TEST BREAKS IF HOSTNAME IS 12 CHARACTERS, MANUALLY CHANGE HOSTNAME IF THIS HAPPENS
        ret = socket.gethostname()
        ret = str.encode(ret)
    elif(msg==0xea7beef):
        ret = 0xCAFEBABE
        ret = struct.pack('!L',ret)
    else:
        print("!serv: msg not recognized: ")
        _print_msg(msg)

    return ret

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
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serv_addr = ('localhost', port)
    print("!serv: starting server on %s %s" % serv_addr)
    s.bind(serv_addr)
    s.listen(1)
    conn, addr = s.accept()
        
    while True:
        msgrecv = conn.recv(MAX_TCP_MSG)
        if(msgrecv == b''): #termination signal is when we get no data apparently, thanks for spelling that out in the instructions.
            break
        msgrecv = struct.unpack('!L',msgrecv)[0]
        print("!serv: Got: ")
        _print_msg(msgrecv)
        msgsend = _convTCPresp(msgrecv)
        print("!serv: Sending: ")
        _print_msg(msgsend)
        conn.send(msgsend)
    conn.close()
    s.close()

def _convUDPresp(msg):
    ret = b''
    if(msg==b'Hi.'):
        ret = b'Hello.'
    elif(msg==b'Nice weather?'):
        ret = b'Too sunny.'
    elif(msg==b'HOSTNAME'):
        ret = str.encode(socket.gethostname())
    elif(msg==b'Ribbit.'):
        ret = b'Frogs are slimy.'
    elif(msg==b'Bye.'):
        ret = b'Later.'
    else:
        print("!serv: msg not recognized: ")
        _print_msg(msg)

    return ret

    
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
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    serv_addr = ('localhost', port)
    print("!serv: starting server on %s %s" % serv_addr)
    s.bind(serv_addr)
    while True:
        msgrecv,cli_addr = s.recvfrom(MAX_UDP_MSG)
        print("!serv: Got: ")
        _print_msg(msgrecv)
        msgsend = _convUDPresp(msgrecv)
        print("!serv: Sending: ")
        _print_msg(msgsend)
        s.sendto(msgsend, cli_addr)
        if(msgrecv == b'Bye.'):
            break
    s.close()
