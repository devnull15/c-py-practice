# JQR Exercises: 3.3.3

```
title: 'Demonstrate skill in creating and using a doubly linked list:'
condition: B
standard: C
sub_items:
    3.3.3.a: creating a doubly linked list with n number of items
    3.3.3.b: navigating through ha doubly linked list
    3.3.3.c: finding the first occurrence of an item in a doubly linked list
    3.3.3.d: sorting the doubly linked list alphanumerically using a function
        pointer
    3.3.3.e: inserting an item into a specific location in a doubly linked list
    3.3.3.f: removing all items from the doubly linked list
    3.3.3.g: destroying a doubly linked list
```

## Instructions


Your task is to implement the functions as defined in the header files within the include directory:
* `include/doubly_linked_lists.h`


## Submission: 

Submit your solutions as
* `src/doubly_linked_lists.c`
          

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
./build/3.3.3_exe
```


Use the following instead of `printf`:
```
volatile int g_arrays_fd_stdout = STDOUT_FILENO;
volatile int g_arrays_fd_stderr = STDERR_FILENO;
volatile int g_arrays_fd_stdin = STDIN_FILENO;

/**
 * @brief provides a wrapper for printf to control where stdout goes
 * 
 * @param format the format string to pass to *printf
 * @param ... the additional variables to pass to *printf
 */
static void local_printf(const char *format, ...)
{
    va_list args; // thanks to https://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html
    va_start(args, format);
    vdprintf(g_arrays_fd_stdout, format, args);
    va_end(args);
}
```
