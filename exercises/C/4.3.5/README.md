# JQR Exercises: 3.3.7

```
title: Demonstrate skill in creating and using a binary search tree
sub_items:
    3.3.7.a: creating a binary search tree with n numbers of items
    3.3.7.b: navigating through a binary search tree
    3.3.7.c: locating an item in a binary tree
    3.3.7.d: Removing selected items from the binary search tree
    3.3.7.e: removing all items from the binary search tree
    3.3.7.g: destroying a binary search tree
```

## Instructions


Your task is to implement the functions as defined in the header files within the include directory:
* `include/bst.h`


## Submission: 

Submit your solutions as
* `src/bst.c`
          

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
./build/3.3.7_exe
```

## Special Instructions

Please note that Item 3.3.7.f has its own [Knowledge exercise, which you can find here](../../Knowledge/3.3.7.f/README.md).
```
  3.3.7.f: describe implementation strategies for a balanced binary search tree
```

