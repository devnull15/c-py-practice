import sys
import socket
import ssl
from cryptography import x509
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives import serialization


BUFF = 1024

"""
    This function will connect to a server at the given hostname and port.

    You should use cafile as the Certificate Authority to validate the server's
    certificate.

    If the certificate is valid, you should receive a message from the server.
    The message will be no more than 1024 bytes long.

    You should encrypt that message using the server's public key and send the
    encrypted message back to the server.

    If the certificate is not valid, you should terminate the connection
    and raise an exception.
    """

def connect_to_server(cafile, hostname, port):
    context = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
    context = ssl.create_default_context(ssl.Purpose.SERVER_AUTH, cafile=cafile)
    #enabled by default via ssl.PROTOCOL_TLS_CLIENT arg in constructor
    #context.verify_mode = ssl.CERT_REQUIRED
    #context.check_hostname = True
    try:
        client = socket.create_connection((hostname,port))
        conn = context.wrap_socket(client, server_hostname=hostname, server_side=False)
        recv = conn.recv(BUFF)
        # print("\nrecvd", file=sys.stderr)
        # print(recv, file=sys.stderr)
        pem_data = ssl.get_server_certificate((hostname,port)).encode("utf8")
        print("pem_data\n%s" % (pem_data), file=sys.stderr)

        pub_key = x509.load_pem_x509_certificate(pem_data, backend=default_backend()).public_key()
        #print("pub_key:\n%s" % (pub_key), file=sys.stderr)
        recv_enc = pub_key.encrypt(recv, padding.OAEP(mgf=padding.MGF1(algorithm=hashes.SHA256()),
                                                      algorithm=hashes.SHA256(),
                                                      label=None))
        #print("recvd encrypted", file=sys.stderr)
        #print(recv_enc, file=sys.stderr)

        #DEBUG
        #priv_key = serialization.load_pem_private_key(pem_data, password=None)
        #decrypted = priv_key.decrypt(recv_enc, padding.OAEP(mgf=padding.MGF1(algorithm=hashes.SHA256()),
        #algorithm=hashes.SHA256(),
        #label=None))
        #print("recvd decrypted", file=sys.stderr)
        #print(decrypted, file=sys.stderr)

        conn.send(recv_enc)
    except Exception as e:
        print(e)
    finally:
        conn.close()
        client.close()
