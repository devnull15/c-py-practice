# JQR Exercises for section 3.1.10

```
title: 'Demonstrate the following skills using pointers:'
condition: C
standard: B
sub_items:
  3.1.10.a: declaring an integer pointer
  3.1.10.b: de-referencing a variable to get its value
  3.1.10.c: printing the address of a variable
  3.1.10.d: assigning a value to a pointer
  3.1.10.e: make use of a function pointer to call another function
  3.1.10.f: make effective use of a pointer arithmetic to traverse an array
```

## Instructions

Your task is to implement the functions as defined in `include/jqr_pointers.h`


## Submission: 

Submit your solution as `src/jqr_pointers.c`.

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
./build/3.1.10_exe
```

