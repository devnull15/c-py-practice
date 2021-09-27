# JQR Exercises for section 3.1.8

```
    3.1.8:
      title: 'Demonstrate the ability to create and implement functions to meet a
        requirement:'
      condition: C
      standard: B
      sub_items:
        3.1.8.a: Proper declarations for created functions
        3.1.8.b: A function that returns a void
        3.1.8.c: A function that is passed an argument by value
        3.1.8.d: A function that is passed an argument by reference
        3.1.8.e: A function that is passed a value by reference
        3.1.8.f: A function that receives input from a user
        3.1.8.g: A function pointer
        3.1.8.h: A recursive function
```

## Instructions


Your task is to implement the functions as defined in the header files within the include directory:
* `include/functions.h`


## Submission: 

Submit your solutions as
* `src/functions.c`
          

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
./build/3.1.8_exe
```
