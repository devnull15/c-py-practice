# JQR Exercises: 3.3.4

```
title: 'Demonstrate skill in creating an d using a circularly linked list:'
condition: B
standard: C
sub_items:
    3.3.4.a: creating a circularly linked list with n number of items
    3.3.4.b: navigating through a circularly linked list
    3.3.4.c: finding the first occurrence of an item in a circularly linked list
    3.3.4.d: sorting the circularly linked list alphanumerically using a function
        pointer
    3.3.4.e: removing selected items from the circularly linked list
    3.3.4.f: inserting an item into a specified location in a circularly linked
        list
    3.3.4.g: removing all items from a circularly linked list
    3.3.4.h: 'destroying a circularly linked list. '
```

## Instructions


Your task is to implement the functions as defined in the header files within the include directory:
* `include/Cllist.h`


## Submission: 

Submit your solutions as
* `src/Cllist.c`
          

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
./build/3.3.4_exe
```

