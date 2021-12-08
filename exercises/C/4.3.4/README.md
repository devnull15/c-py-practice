# JQR Exercises: 3.3.6

```
title: 'Demonstrating skill in creating and using a tree:'
condition: B
standard: C
sub_items:
    3.3.6.a: creating a tree with n number of items
    3.3.6.b: navigating through a tree
    3.3.6.c: finding the first occurrence of an item in a tree
    3.3.6.d: removing selected items from the tree
    3.3.6.e: inserting an item into a specified location in a tree
    3.3.6.f: removing all items from the tree
    3.3.6.g: destroying a tree
```

## Instructions


Your task is to implement the functions as defined in the header files within the include directory:
* `include/Tree.h`


## Submission: 

Submit your solutions as
* `src/Tree.c`
          

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
./build/3.3.6_exe
```

