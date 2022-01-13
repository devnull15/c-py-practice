#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include "../include/Graph.h"

#define EDGE_WEIGHT 1

/* graph_new() creates a new graph
    Args:
        No arguments
    Return:
        Graph * (also equal to graph*) to a newly allocated graph.
*/
Graph *graph_new(void) {
  Graph* new_graph = (Graph*) calloc(1, sizeof(Graph));
  assert(NULL != new_graph);
  
  return new_graph;
}

/* graph_add_node() adds node to existing graph
    Args:
        Graph * pGraph : the graph to add the element to
        node_object obj : the specific data object element to add
    Return:
        bool: true on success, false on failure
*/
bool graph_add_node(Graph *pGraph, node_object *obj) {
  if(pGraph == NULL || obj == NULL) { return false; }

  node *prev = NULL;
  node *n = pGraph->nodes;
  
  while(n != NULL) {
    prev = n;
    n = n->next;
  }

  n = calloc(1,sizeof(node));
  assert(NULL != n);
  n->obj = obj;
  if(prev != NULL) { prev->next = n; }
  if(pGraph->nodes == NULL) { pGraph->nodes = n; }
  
  return true;
}

/* graph_add_edge() add edge to existing graph
    Args:
        Graph * pGraph  : the pgraph to add the element to
        node_obj *src_obj: the source node object to add an edge to
        node_obj *dst_obj: the edge's destination node object
    Return:
        bool: true on success, false on failure
*/


bool graph_add_edge(Graph *pGraph, node_object *src_obj, node_object *dst_obj) {
  if(pGraph == NULL || src_obj == NULL || dst_obj == NULL) { return false; }
    
  node *src = graph_find(pGraph, src_obj);
  node *dst = graph_find(pGraph, dst_obj);
  if(src == NULL || dst == NULL) { return false; }

  edge *prev = NULL;
  edge *e = src->edge;
  
  while(e != NULL) {
    prev = e;
    e = e->edge_next;
  }
  e = calloc(1, sizeof(edge));
  assert(NULL != e);
  e->weight = EDGE_WEIGHT;
  e->dst_node = dst;
  if(prev != NULL) { prev->edge_next = e; }
  if(src->edge == NULL) { src->edge = e; }  

  return true;
}


/* graph_find() searches a graph for the first occurrence of a given value
    Args:
        Graph * pGraph : the pgraph to find the element in
        node_object *data_obj : the specific data object element to find
    Return:
        node *: pointer to node if found. NULL if node not found.
*/
node *graph_find(Graph *pGraph, node_object *data_obj) {
  if(pGraph == NULL) { return false; }

  node *n = pGraph->nodes;
  
  while(n != NULL && n->obj != data_obj) {
    n = n->next;
  }
  if(n == NULL) { return NULL; }

  return n;
}


/* graph_rm_node() removes the node and all edges to and from it from a graph
    Args:
        Graph * pGraph : the Graph * to remove the node from
        node_object *data_obj : the specific data object to remove
    Return:
        bool: true on success, false on failure
*/

void _rm_all_edges(node *n) {
  edge *e = n->edge;
  edge *e_next = NULL;
  while(e != NULL) {
    e_next = e->edge_next;
    free(e);
    e = e_next;
  }
  n->edge = NULL;
  return;
}

bool graph_rm_node(Graph *pGraph, node_object *data_obj) {
  if(pGraph == NULL) { return false; }

  node *prev = NULL;
  node *n = pGraph->nodes;
  
  while(n != NULL && n->obj != data_obj) {
    prev = n;
    n = n->next;
  }
  if(n == NULL) { return false; }

  //kills all edges FROM node
  _rm_all_edges(n);

  //kills all edges TO node
  node *cur = pGraph->nodes;
  while(cur != NULL) {
    graph_rm_edge(pGraph, cur->obj, data_obj);
    cur = cur->next;
  }


  if(prev != NULL) { prev->next = n->next; }
  if(pGraph->nodes == n) { pGraph->nodes = n->next; }
  free(n->obj);
  free(n);
  n = NULL;
  
  return true;
}


/* graph_rm_edge() removes all edges from src_obj to dst_obj nodes within pGraph
    Args:
        Graph * pGraph : the Graph * to remove the edges from
        node_object *src_obj: the source object of the edges to remove
        node_object *dst_obj: the destination object of the edges to remove
    Return:
        bool: true on success (edges removed), false on failure (no edges removed)
*/
bool graph_rm_edge(Graph *pGraph, node_object *src_obj, node_object *dst_obj) {
  if(pGraph == NULL || src_obj == NULL || dst_obj == NULL) { return false; }

    
  node *src = graph_find(pGraph, src_obj);
  if(src == NULL) { return false; }

  bool rmd = false;
  edge *prev = NULL;
  edge *e = src->edge;
  edge *next = NULL;

  
  while(e != NULL) {
    if(e->dst_node->obj == dst_obj) {

      next = e->edge_next;
      if(e == src->edge) { src->edge = next; }
      free(e);
      e = next;
      rmd = true;
      if(prev != NULL) { prev->edge_next = next; }
    }
    prev = e;
    if(e != NULL) { e = e->edge_next; }
  }

  
  return rmd;
}


/* graph_destroy() destroys a graph
    Args:
        Graph * pGraph : Graph * to destroy
    Return:
        bool : true on success, false on failure
*/
bool graph_destroy(Graph *pGraph) {
  if(pGraph == NULL) { return false; }
  node *n = pGraph->nodes;
  node *n_next = NULL;

  while(n != NULL) {
    n_next = n->next;
    graph_rm_node(pGraph, n->obj);
    n = n_next;
  }

  free(pGraph);
  pGraph = NULL;
  return true;
}

