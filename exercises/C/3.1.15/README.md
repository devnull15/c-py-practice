# JQR Exercises for section 3.1.15

```
title: demonstrate skill in creating and implementing a state machine
condition: C
standard: B
sub_items: {}
```



## Instructions


Write a state machine that interprets an esoteric language. More information
available in header file. You will likely also find more details about this esoteric language in public resources if you have further questions. 

Your task is to implement the functions as defined in the header files within the include directory:
* `include/bfe.h`



## Submission: 

Submit your solutions as
* `src/bfe.c`
          

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
./build/3.1.15_exe
```


