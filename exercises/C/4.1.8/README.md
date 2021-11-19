# JQR Exercises for section 3.1.7

```
title: 'Demonstrate the ability to perform file management operations in C:'
condition: C
standard: B
sub_items:
  3.1.7.a: Open an existing file
  3.1.7.b: Read data from a file
  3.1.7.c: Write data to a file
  3.1.7.d: Modify data to a file
  3.1.7.e: Close an open file
  3.1.7.f: Print file information to the console
  3.1.7.g: Create a new file
  3.1.7.h: Append data to an existing file
  3.1.7.i: Delete a file
  3.1.7.j: Determine the size of a file
  3.1.7.k: Determine the location within a file
  3.1.7.l: Insert data into an existing file
```

## Instructions


Your task is to implement the functions as defined in the header files within the include directory:
* `include/file_io.h`


## Submission: 

Submit your solutions as
* `src/file_io.c`
          

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
./build/3.1.7_exe
```
