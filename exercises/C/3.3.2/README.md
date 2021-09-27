# JQR Exercises: 3.3.2

```
title: 'Demonstrate skill in creating and using a linked list:'
condition: B
standard: C
sub_items:
    3.3.2.a: Creating a linked list with n number of items
    3.3.2.b: Navigating through a linked list
    3.3.2.c: Retrieving the first occurrence of an item in a linked list
    3.3.2.d: Sorting the linked list alphanumerically using a function pointer
    3.3.2.e: Removing selected items from the linked list
    3.3.2.f: Inserting an item into a specified location in a linked list
    3.3.2.g: Removing all items from the linked list
    3.3.2.h: Destroying a linked list
```

## Instructions


Your task is to implement the functions as defined in the header files within the include directory:
* `include/LinkedList.h`


## Submission: 

Submit your solutions as
* `src/LinkedList.c`
          

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
./build/3.3.2_exe
```
