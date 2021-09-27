# JQR Exercises: 3.2.8
```
      title: Demonstrate skill in using networking methods and functions, accounting
        for endianness
      condition: C
      standard: C
      sub_items:
        3.2.8.a: socket()
        3.2.8.b: send()
        3.2.8.c: recv()
        3.2.8.d: sendto()
        3.2.8.e: recvfrom()
        3.2.8.f: bind()
        3.2.8.g: listen()
        3.2.8.h: connect()
        3.2.8.i: accept()
```

## Instructions


Your task is to implement the methods as defined in the doc strings
located in the folder `./doc_strings`. 

All of your implementation should occur in 

`./src/__init__.py`

You should then submit your file in the src folder as your solution.
          

## CI Pipeline

Note: if your CI pipeline does not run when you commit, please ensure you have made a merge request to merge
your branch into the primary branch. This will trigger the rules for the CI Job configured for this item.

## Testing Locally

You can test your submission locally by running the 
following command from the root of the exercise folder

```
python3 tests/__init__.py
```

