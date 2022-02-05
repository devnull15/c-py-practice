# JQR Exercises for section 3.1.12

```
title: 'Demonstrate skill in using networking functions, accounting for endianness '
condition: C
standard: B
sub_items:
  3.1.12.a: socket()
  3.1.12.b: send()
  3.1.12.c: recv()
  3.1.12.d: sendto()
  3.1.12.e: recvfrom()
  3.1.12.f: bind()
  3.1.12.g: connect()
  3.1.12.h: accept()
  3.1.12.i: getsockopt()
  3.1.12.j: setsockopt()
  3.1.12.k: getaddrinfo()
  3.1.12.l: gethostname
  3.1.12.m: sethostname
  3.1.12.n: struct sockaddr{}
  3.1.12.o: struct sockaddr_in{}
  3.1.12.p: struct sockaddr_un{}
```

## Instructions

Your task is to implement the functions as defined in the header files within the include directory:
* `include/broadcast_udp.h`
* `include/converse_tcp.h`
* `include/converse_udp.h`
* `include/getsockopt.h`
* `include/send_tcp.h`
* `include/send_udp.h`
* `include/send_unix.h`
* `include/set_hostname.h`


## Submission: 

Submit your solutions as
* `src/broadcast_udp.c`
* `src/converse_tcp.c`
* `src/converse_udp.c`
* `src/getsockopt.c`
* `src/send_tcp.c`
* `src/send_udp.c`
* `src/send_unix.c`
* `src/set_hostname.c`
            


## CI Pipeline

Note: if your CI pipeline does not run when you commit, please ensure you have made a merge request to merge
your branch into the primary branch. This will trigger the rules for the CI Job configured for this item.

## Build instructions

To test your C code locally, from this folder, run the following command:

```
bash ../../../scripts/build.sh
```

Alternatively, you can run the cmake build commands directly with:

```
cmake -H. -Bbuild
cmake --build build
```

After your build is complete, run the executable

```
export LD_PRELOAD=./build/libhostname/libhostname.so
./build/3.1.12_exe
```

Note the use of the shared library above to allow the test case to hook the hostname functions for testing. 

