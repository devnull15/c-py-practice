# JQR Exercises: 3.3.10

```
title: 'Demonstrate skill in creating and using a weighted graph:'
condition: B
standard: C
sub_items:
    3.3.10.a: defining the structures required for graphs
    3.3.10.b: creating a graph with n number of nodes
    3.3.10.c: adding n number of edges to a graph
    3.3.10.d: finding a node within an existing graph
    3.3.10.e: finding an edge within a graph
    3.3.10.f: removing an edge from a graph
    3.3.10.g: removing a node and all of its edges from a graph
    3.3.10.h: calculate the weight of a path within a graph
    3.3.10.i: destroy the graph
```

## Instructions


Your task is to implement the functions as defined in the header files within the include directory:
* `include/Graph.h`


## Submission: 

Submit your solutions as
* `src/Graph.c`
          

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
./build/3.3.10_exe
```
