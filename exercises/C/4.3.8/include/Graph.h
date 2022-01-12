#ifndef JQR_3310_GRAPH_H
#define JQR_3310_GRAPH_H

#include <stdbool.h>

// The edge node is a linked list of edges
typedef struct _edge edge;
/*
 * The standard node contains node objects with a single unsiged integer for
 * the sake of this exercise. It also contains the 'head' edge node.
 */
typedef struct _node node;

typedef struct _node_object
{
    int data;
} node_object;

struct _edge
{
    int weight;
    node *dst_node;
    edge *edge_next;
};

struct _node
{
    node_object *obj;
    edge *edge;
    node *next;
};

/*
 * This "overarching" struct is intended to encapsulate the head node.
 */
typedef struct _AdjacencyList
{
    node *nodes;
} Graph;

// Graph Methods to implement

/* How are you going to create the node_objects? HINT: Put each node_object on the heap.
 * They are to be cleaned up in graph_destroy()
 * Remember, you are allowed to generate any helper functions that you may need.
 */

/* graph_new() creates a new graph
    Args:
        No arguments
    Return:
        Graph * (also equal to graph*) to a newly allocated graph.
*/
Graph *graph_new(void);

/* graph_add_node() adds node to existing graph
    Args:
        Graph * pGraph : the graph to add the element to
        node_object obj : the specific data object element to add
    Return:
        bool: true on success, false on failure
*/
bool graph_add_node(Graph *pGraph, node_object *obj);

/* graph_add_edge() add edge to existing graph
    Args:
        Graph * pGraph  : the pgraph to add the element to
        node_obj *src_obj: the source node object to add an edge to
        node_obj *dst_obj: the edge's destination node object
    Return:
        bool: true on success, false on failure
*/
bool graph_add_edge(Graph *pGraph, node_object *src_obj, node_object *dst_obj);

/* graph_find() searches a graph for the first occurrence of a given value
    Args:
        Graph * pGraph : the pgraph to find the element in
        node_object *data_obj : the specific data object element to find
    Return:
        node *: pointer to node if found. NULL if node not found.
*/
node *graph_find(Graph *pGraph, node_object *data_obj);

/* graph_rm_node() removes the node and all edges to and from it from a graph
    Args:
        Graph * pGraph : the Graph * to remove the node from
        node_object *data_obj : the specific data object to remove
    Return:
        bool: true on success, false on failure
*/
bool graph_rm_node(Graph *pGraph, node_object *data_obj);

/* graph_rm_edge() removes all edges from src_obj to dst_obj nodes within pGraph
    Args:
        Graph * pGraph : the Graph * to remove the edges from
        node_object *src_obj: the source object of the edges to remove
        node_object *dst_obj: the destination object of the edges to remove
    Return:
        bool: true on success (edges removed), false on failure (no edges removed)
*/
bool graph_rm_edge(Graph *pGraph, node_object *src_obj, node_object *dst_obj);

/* graph_destroy() destroys a graph
    Args:
        Graph * pGraph : Graph * to destroy
    Return:
        bool : true on success, false on failure
*/
bool graph_destroy(Graph *pGraph);

#endif
