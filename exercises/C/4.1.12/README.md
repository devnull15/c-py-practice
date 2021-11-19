# JQR Exercises for section 3.1.11

```
title: 'Demonstrate skill in creating and implementing conditional statements, expressions,
  and constructs:'
condition: C
standard: B
sub_items:
  3.1.11.a: for loop
  3.1.11.b: while loop
  3.1.11.c: do while loop
  3.1.11.d: if statement
  3.1.11.e: if->else statement
  3.1.11.f: if->else if->else statement
  3.1.11.g: switch statement
  3.1.11.h: effective use of goto labels to construct a single exit point within a
    function
```

## Instructions

Your task is to implement the functions as defined in `include/jqr_conditionals.h`


## Submission: 

Submit your solution as `src/jqr_conditionals.c`.

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
./build/3.1.11_exe
```


