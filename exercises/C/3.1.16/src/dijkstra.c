#include "../include/dijkstra.h"
#include <stdio.h>


void _print_arrays(int *visited, uint32_t *dists, size_t len) {
  fprintf(stderr, "visited\tdists\n");
  for (size_t i = 0; i < len; i++) {
    fprintf(stderr, "%ul\t%ul\n", visited[i], dists[i]);
  }
}

//this would be better as a min priority queue, but I'm too lazy for that
int _pick_closest(int *visited, uint32_t *dists, size_t len) {
  size_t min, minI;
  min = UINT32_MAX;
  for(size_t i = 0; i < len; i++) {
    if(visited[i] == 0 &&  dists[i] <= min) {
      min = dists[i];
      minI = i;
    }
  }
  return minI;
}

void _print_graph(uint32_t **graph, size_t len) {
  for(size_t i = 0; i < len; i++){
    for(size_t j = 0; j < len; j++) {
      fprintf(stderr, " %i ", graph[i][j]);
    }
    fprintf(stderr,"\n");
  }
}

/**
 * @brief Use Dijkstra's Algorithm to find the shortest distance from the source
 * to other vertices in the graph.
 * 
 * The graph will be provided as an adjacency matrix.
 * 
 * Your function should allocate memory that will be free'd by the caller.
 * 
 * If the are nodes that cannot be reached from the source, then the distance 
 * to those nodes should be UINT32_MAX.
 * 
 * If there is an error you should return a null pointer.
 * 
 * @param graph The graph
 * @param src The source vertex
 * @param num_vertices The number of vertices in the graph
 * @return uint32_t* Array of distances from the source to every node.
 */
uint32_t* dijkstra(uint32_t** graph, size_t src, size_t num_vertices) {

  //NULL Graph
  if(graph == NULL || num_vertices == 0) { return NULL; }
  
  int visited[num_vertices]; // marks if a vertex has been visited
  uint32_t *dists = malloc(sizeof(uint32_t)*num_vertices);
  
  //initialize every distance to max and visited to false except for src
  for(size_t i = 0; i < num_vertices; i++) {
    if(i == src) { dists[i] = 0; }
    else { dists[i] = UINT32_MAX; }
    visited[i] = 0;
  }

  //debug
  _print_arrays(visited, dists, num_vertices);
  _print_graph(graph, num_vertices);
  //debug

  //visit every node
  for(size_t i = 0; i < num_vertices; i++) {

    //pick the closest node as the next to visit
    int curr =  _pick_closest(visited,dists, num_vertices);

   // mark as visited
   visited[curr] = 1;

   //update edge distances
   for(size_t j = 0; j < num_vertices; j++) {

     // update distance for nodes that are:
     // 1) not visited
     // 2) has an edge connected to curr
     // 3) and the distance of the current node + the new edge is smaller than the last recorded distance in dists
     // 4) The current node is not unreachable ( != UINT32_MAX )
     if(!visited[j] &&
	graph[curr][j] &&
	dists[curr] != UINT32_MAX &&
	dists[curr] + graph[curr][j] < dists[j]) {
       dists[j] = dists[curr] + graph[curr][j];
     }
   }
  }
  return dists;
}
