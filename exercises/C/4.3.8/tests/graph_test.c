/*
    Reference the CUnit Users Guide:
    http://cunit.sourceforge.net/doc/writing_tests.html#tests

    Step 1. Create the test cases
    Step 2. _add the test suite to the registry
    Step 3. _add the test to the suite.
*/

#include "../include/Graph.h"
#include <stdlib.h>
#include <CUnit/CUnitCI.h>
#include <CUnit/CUnit.h>
#include <assert.h>

// These constants are used for data structres and give a default size for containers like lists
#define DEFAULT_MIN_CONTAINER_SIZE 5
#define DEFAULT_MAX_CONTAINER_SIZE 20

// This give a sane small random integer, but large enough that should be able to fill
// the largest random container uniquely without too many retries due to collisions
#define DEFAULT_MIN_VALUE 1
#define DEFAULT_MAX_VALUE 100
#define DEFAULT_RANGE (DEFAULT_MAX_VALUE - DEFAULT_MIN_VALUE + 1)

// Keep track of the random values we've generated.
char set[DEFAULT_RANGE] = {0, };

//Global graph variable we define so graph_destroy is not needed in tests of other functions
Graph * test_graph;

/**
 * @brief returns a random number in range [min, max]
 */
int rand_range(int min, int max)
{
    int range_size = max - min + 1;
    return rand() % range_size + min;
}
static inline int rand_container_size(void)
{
    return rand_range(DEFAULT_MIN_CONTAINER_SIZE, DEFAULT_MAX_CONTAINER_SIZE);
}

static inline int rand_value(void)
{
    int val = rand_range(DEFAULT_MIN_VALUE, DEFAULT_MAX_VALUE);
    while (set[val - DEFAULT_MIN_VALUE])
    {
        val = rand_range(DEFAULT_MIN_VALUE, DEFAULT_MAX_VALUE);
    }
    set[val - DEFAULT_MIN_VALUE] = 1;
    return val;
}

// HELPER FUNCTIONS
node_object *new_obj(int value)
{
  node_object *pNode = (node_object *)calloc(1, sizeof(node_object));
  if(NULL == s) {
    fprintf(stderr, "!!! calloc error in new_obj\n");
    return NULL;
  }
  
    pNode->data = value;

    return pNode;
}

int edge_count(Graph * curr_graph)
{
    int count = 0;
    node * curr = curr_graph->nodes;
    edge * curr_edge;
    while(curr != NULL)
    {
        curr_edge = curr->edge;
        while(curr_edge != NULL)
        {
            count++;
            curr_edge = curr_edge->edge_next;
        }
        curr = curr->next;
    }

    return count;
}

int node_count(Graph * curr_graph)
{
    int count = 0;
    node * curr = curr_graph->nodes;
    while(curr != NULL)
    {
        count++;
        curr = curr->next;
    }
    return count;
}


/* ------ 1. Test cases ------ */
static void test_graph_new(void)
{
    printf("Inside test graph new\n");
    test_graph = graph_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_graph);
    CU_ASSERT_PTR_NULL_FATAL(test_graph->nodes);
}

static void test_graph_add_node(void)
{

    int value = rand_container_size();
    
    int * values = calloc(value, sizeof(int));
    if(NULL == values) {
      fprintf(stderr, "!!! calloc error in test_graph_add_node\n");
      return;
    }

    for(int i = 0; i < value; i++)
    {
        values[i] = rand_value();
        node_object *pObj = new_obj(values[i]);
        CU_ASSERT_TRUE(graph_add_node(test_graph, pObj));
    }

    CU_ASSERT_TRUE(node_count(test_graph) == value);

    free(values);

}

static void test_graph_add_null(void)
{

    node_object *pObj = new_obj(rand_value());

    CU_ASSERT_FALSE(graph_add_node(NULL, pObj));

    free(pObj);
}

static void test_graph_add_null_node(void)
{

    CU_ASSERT_FALSE(graph_add_node(test_graph, NULL));

}

static void test_graph_add_edge(void)
{

    node * start = test_graph->nodes;
    node * temp = start;
    node * inner = temp;
    while(temp)
    {
        while(inner)
        {
            if(temp != inner)
            {
                CU_ASSERT_TRUE(graph_add_edge(test_graph, temp->obj, inner->obj));
            }
            inner = inner->next;
        }
        inner = start;
        temp = temp->next;
    }

    node_object *pObj = new_obj(rand_value());

    CU_ASSERT_FALSE(graph_add_edge(test_graph, test_graph->nodes->obj, pObj));
    CU_ASSERT_FALSE(graph_add_edge(test_graph, pObj, test_graph->nodes->obj));

    CU_ASSERT_TRUE(edge_count(test_graph) == (node_count(test_graph) * (node_count(test_graph) - 1)));

    free(pObj);

}

static void test_graph_add_edge_null(void)
{
    CU_ASSERT_FALSE(graph_add_edge(NULL, &(node_object){1}, &(node_object){2}));
}

static void test_graph_add_edge_null_src(void)
{
    CU_ASSERT_FALSE(graph_add_edge(test_graph, NULL, test_graph->nodes->obj));
}

static void test_graph_add_edge_null_dst(void)
{

    CU_ASSERT_FALSE(graph_add_edge(test_graph, test_graph->nodes->obj, NULL));

}

static void test_graph_find(void)
{

    int value = node_count(test_graph);
    int idx = rand() % value;

    node * curr = test_graph->nodes;

    for (int i = 0; i < idx; i++)
    {
        curr = curr->next;
    }

    node_object * find_obj = curr->obj;

    CU_ASSERT_PTR_NOT_NULL_FATAL(find_obj);
    CU_ASSERT_TRUE(graph_find(test_graph, find_obj) == curr);

    node_object *pObj = new_obj(rand_value());
    CU_ASSERT_PTR_NULL(graph_find(test_graph, pObj));
    free(pObj);

}

static void test_graph_find_null(void)
{

    CU_ASSERT_PTR_NULL(graph_find(NULL, test_graph->nodes->obj));

}

static void test_graph_find_null_node(void)
{

    node_object *found_obj = NULL;

    CU_ASSERT_PTR_NULL(graph_find(test_graph, found_obj));

}

static void test_graph_rm_node(void)
{
    int value = node_count(test_graph);
    int idx = rand() % value;

    node * curr = test_graph->nodes;

    for (int i = 0; i < idx; i++)
    {
        curr = curr->next;
    }

    node_object * obj_to_rm = curr->obj;

    CU_ASSERT_PTR_NOT_NULL_FATAL(obj_to_rm);
    CU_ASSERT_TRUE(graph_rm_node(test_graph, obj_to_rm));
    CU_ASSERT_TRUE(node_count(test_graph) == value - 1);
    CU_ASSERT_TRUE(edge_count(test_graph) == (node_count(test_graph) * (node_count(test_graph) - 1)));

    node_object *pObj = new_obj(rand_value());
    CU_ASSERT_FALSE(graph_rm_node(test_graph, pObj));
    free(pObj);

}

static void test_graph_rm_null_node(void)
{

    CU_ASSERT_FALSE(graph_rm_node(NULL, test_graph->nodes->obj));

}

static void test_graph_rm_edge(void)
{

    int value = node_count(test_graph);
    int src_to_rm = rand() % value;
    int dst_to_rm = rand() % value;
    while (src_to_rm == dst_to_rm)
    {
        dst_to_rm = rand() % value;
    }

    node * curr = test_graph->nodes;
    node_object *src = NULL;
    node_object *dst = NULL;

    int i = 0;
    while(curr != NULL)
    {
        if(src_to_rm == i)
        {
            src = curr->obj;
        }
        else if(dst_to_rm == i)
        {
            dst = curr->obj;
        }
        curr = curr->next;
        i++;
    }

    CU_ASSERT_PTR_NOT_NULL_FATAL(dst);
    CU_ASSERT_PTR_NOT_NULL_FATAL(src);

    CU_ASSERT_TRUE(graph_rm_edge(test_graph, src, dst));
    CU_ASSERT_FALSE(graph_rm_edge(test_graph, src, dst));
    CU_ASSERT_TRUE(edge_count(test_graph) == (node_count(test_graph) * (node_count(test_graph) - 1) - 1));

}

static void test_graph_rm_edge_null(void)
{

    CU_ASSERT_FALSE(graph_rm_edge(NULL, test_graph->nodes->obj, test_graph->nodes->edge->dst_node->obj));

}

static void test_graph_rm_edge_null_src(void)
{
    int edges = edge_count(test_graph);
    int value = node_count(test_graph);
    int dst_to_rm = rand() % value;

    node * curr = test_graph->nodes;
    node_object *src = NULL;
    node_object *dst = NULL;

    int i = 0;
    while(curr != NULL)
    {
        if(dst_to_rm == i)
        {
            dst = curr->obj;
        }
        curr = curr->next;
        i++;
    }

    CU_ASSERT_PTR_NOT_NULL_FATAL(dst);
    CU_ASSERT_FALSE(graph_rm_edge(test_graph, src, dst));
    CU_ASSERT_TRUE(edge_count(test_graph) == edges);

}

static void test_graph_rm_edge_null_dst(void)
{
    int edges = edge_count(test_graph);
    int value = node_count(test_graph);
    int src_to_rm = rand() % value;

    node * curr = test_graph->nodes;
    node_object *src = NULL;
    node_object *dst = NULL;

    int i = 0;
    while(curr != NULL)
    {
        if(src_to_rm == i)
        {
            src = curr->obj;
        }
        curr = curr->next;
        i++;
    }

    CU_ASSERT_PTR_NOT_NULL_FATAL(src);
    CU_ASSERT_FALSE(graph_rm_edge(test_graph, src, dst));
    CU_ASSERT_TRUE(edge_count(test_graph) == edges);

}

static void test_graph_destroy(void)
{
    CU_ASSERT_TRUE(graph_destroy(test_graph));
}

static void test_graph_destroy_null(void)
{
    CU_ASSERT_FALSE(graph_destroy(NULL));
}
