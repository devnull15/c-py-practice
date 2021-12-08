# JQR Exercises for section 3.1.24

```
title: 'Demonstrate the ability to manage memory in multi-threaded programs that make
  effective uses of constructs including:'
condition: C
standard: B
sub_items:
  3.1.24.a: threads
  3.1.24.b: semaphores
  3.1.24.c: mutexes
```



## Instructions


Your task is to implement the functions as defined in the header files within the include directory:
* `include/threadpool.h`


## Submission: 

Submit your solutions as
* `src/threadpool.c`
          

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
export LD_PRELOAD=./build/libmalloc/libmalloc.so
./build/3.1.24_exe
```

NOTE: you should preload the shared library so the test cases are able to test how you handle malloc failures. 

Valgrind can preload in this fashion:

```
 valgrind --leak-check=full -s --soname-synonyms=somalloc=libmalloc/libmalloc.so ./build/3.1.24_exe
```


## References:

* See the following video for a great threadpool tutorial: https://www.youtube.com/watch?v=Pg_4Jz8ZIH4


