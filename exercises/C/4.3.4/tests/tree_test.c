/*
    Reference the CUnit Users Guide:
    http://cunit.sourceforge.net/doc/writing_tests.html#tests

    Step 1. Create the test cases
    Step 2. _add the test suite to the registry
    Step 3. _add the test to the suite.
*/

#include "../include/Tree.h"
#include <stdlib.h>
#include <CUnit/CUnitCI.h>
#include <CUnit/CUnit.h>
#include "TreeHelp.h"
#include <assert.h>

#define MIN_RAND_TREE_SIZE 5
#define MAX_RAND_TREE_SIZE 15

#define MIN_RAND_NODE_VALUE 1
#define MAX_RAND_NODE_VALUE 100

// These constants are used for data structres and give a default size for containers like lists
#define DEFAULT_MIN_CONTAINER_SIZE 5
#define DEFAULT_MAX_CONTAINER_SIZE 20

// This give a sane small random integer, but large enough that should be able to fill
// the largest random container uniquely without too many retries due to collisions
#define DEFAULT_MIN_VALUE 1
#define DEFAULT_MAX_VALUE 100

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
    return rand_range(DEFAULT_MIN_VALUE, DEFAULT_MAX_VALUE);
}

static int
count_nodes(Tree_node *node)
{
    int count = 0;
    if (node)
    {
        count = 1;
        if (node->left)
        {
            count += count_nodes(node->left);
        }

        if (node->right)
        {
            count += count_nodes(node->right);
        }
    }

    return count;
}

/*------ 1. Test cases ------ */
static void test_tree_new(void)
{

    Tree *tree = tree_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(tree);
    CU_ASSERT_PTR_NULL_FATAL(tree->head);

    tree_destroy(tree);
}

static void
test_tree_add(void)
{

    Tree *tree = tree_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(tree);

    int total_nodes = rand_container_size();

    for (int idx = 0; idx < total_nodes; idx++)
    {
        int new_num = rand_value();
        CU_ASSERT_TRUE(tree_add(tree, new_num));
    }

    int node_count = 0;
    Tree_node *temp = tree->head;
    CU_ASSERT_PTR_NOT_NULL_FATAL(temp);
    node_count = count_nodes(temp);

    CU_ASSERT_EQUAL(total_nodes, node_count);

    tree_destroy(tree);
}

static void
test_tree_add_to_null(void)
{

    CU_ASSERT_FALSE(tree_add(NULL, rand_range(MIN_RAND_NODE_VALUE, MAX_RAND_NODE_VALUE)));
}

static void
test_tree_find(void)
{
    // Build tree to test.
    int node_count = rand_container_size();
    int node_to_find_index = rand_range(0, node_count - 1);
    int int_to_find = 0;

    Tree *test_tree = tree_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_tree);

    for (int idx = 0; idx < node_count; idx++)
    {
        int num = rand_range(MIN_RAND_NODE_VALUE, MAX_RAND_NODE_VALUE);
        tree_add(test_tree, num);

        if (idx == node_to_find_index)
        {
            int_to_find = num;
        }
    }
    // End of test tree build

    Tree_node *ret_node = NULL;
    ret_node = tree_find(test_tree, int_to_find);

    CU_ASSERT_PTR_NOT_NULL_FATAL(ret_node);
    CU_ASSERT_EQUAL(ret_node->data, int_to_find);
    tree_destroy(test_tree);
}

static void
test_tree_find_nothing(void)
{
    // Build Tree to test.
    int node_count = rand_container_size();
    Tree *test_tree = tree_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_tree);

    for (int idx = 0; idx < node_count; idx++)
    {
        tree_add(test_tree, rand_range(MIN_RAND_NODE_VALUE, MAX_RAND_NODE_VALUE));
    }
    // End of test Tree build

    CU_ASSERT_PTR_NULL(tree_find(test_tree, MAX_RAND_NODE_VALUE + 1));
    tree_destroy(test_tree);
}

static void
test_tree_find_no_tree(void)
{

    CU_ASSERT_PTR_NULL(tree_find(NULL, rand_value()));
}

static void
test_tree_rm_head(void)
{

    int first_data = 0;
    // Build Linked list to test.
    int node_count = rand_range(MIN_RAND_TREE_SIZE, MAX_RAND_TREE_SIZE);

    Tree *test_tree = tree_new();

    for (int idx = 0; idx < node_count; idx++)
    {
        int num = rand_range(MIN_RAND_NODE_VALUE, MAX_RAND_NODE_VALUE);
        tree_add(test_tree, num);

        if (0 == idx)
        {
            first_data = num;
        }
    }
    // End of test list build

    Tree_node *old_head = tree_rm_head(test_tree);

    CU_ASSERT_PTR_NOT_NULL_FATAL(old_head);
    CU_ASSERT_EQUAL(old_head->data, first_data);
    free(old_head);
    tree_destroy(test_tree);
}
/**
 *  This test looks to check after removal of the head node that any child
 *  nodes that may have been on the head are retained.
 */
static void
test_tree_rm_head_to_empty(void)
{

    // Build Linked list to test.
    int node_count = rand_range(MIN_RAND_TREE_SIZE, MAX_RAND_TREE_SIZE);

    Tree *test_tree = tree_new();

    for (int idx = 0; idx < node_count; idx++)
    {
        int num = rand_range(MIN_RAND_NODE_VALUE, MAX_RAND_NODE_VALUE);
        tree_add(test_tree, num);

    }
    // End of test list build
    Tree_node *old_head = NULL;
    for (int idx = 0; idx < node_count; idx++)
    {
        old_head = tree_rm_head(test_tree);
        CU_ASSERT_PTR_NOT_NULL_FATAL(old_head);
        free(old_head);
        old_head = NULL;
    }
    old_head = tree_rm_head(test_tree);
    CU_ASSERT_PTR_NULL_FATAL(old_head);
    tree_destroy(test_tree);
}

static void
test_tree_rm_head_from_null(void)
{

    CU_ASSERT_PTR_NULL(tree_rm_head(NULL));
}

static void
test_tree_rm_head_no_nodes(void)
{

    // Build Linked list to test.
    Tree *empty_tree = tree_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(empty_tree);
    CU_ASSERT_PTR_NULL(tree_rm_head(empty_tree));

    tree_destroy(empty_tree);
}

static void
test_tree_destroy(void)
{

    // Build Tree to test.
    int node_count = rand_container_size();

    Tree *test_tree = tree_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_tree);

    for (int idx = 0; idx < node_count; idx++)
    {
        tree_add(test_tree, rand_range(MIN_RAND_NODE_VALUE, MAX_RAND_NODE_VALUE));
    }

    CU_ASSERT_TRUE(tree_destroy(test_tree));
}

/**
*   This test method looks to see if the add method will at some point add to both
*   sides of the tree. Both the left and right sides. Not doing so will have a 
*   tree that is just a linked list.
*/

static void
test_is_tree_not_list(void)
{

    Tree *tree = tree_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(tree);

    int total_nodes = rand_container_size() + 3;

    for (int idx = 0; idx < total_nodes; idx++)
    {
        int new_num = rand_value();
        CU_ASSERT_TRUE(tree_add(tree, new_num));
    }
    CU_ASSERT_TRUE(tree_add(tree, tree->head->data - 1));
    CU_ASSERT_TRUE(tree_add(tree, tree->head->data + 1));
    total_nodes += 2;

    //    int node_count = 0;
    Tree_node *temp = tree->head;
    if (NULL != temp)
    {
        int left_count = 0;
        int right_count = 0;
        if (NULL != temp->left)
        {
            left_count = count_nodes(temp->left);
        }
        if (NULL != temp->right)
        {
            right_count = count_nodes(temp->right);
        }
        printf("Left: %d\n", left_count);
        CU_ASSERT_NOT_EQUAL(total_nodes - 1, left_count);

        printf("Right: %d\n", right_count);
        CU_ASSERT_NOT_EQUAL(total_nodes - 1, right_count);

        CU_ASSERT_NOT_EQUAL(left_count, 0);
        CU_ASSERT_NOT_EQUAL(right_count, 0);
    }
    else
    {
        CU_FAIL();
    }
    tree_destroy(tree);
}
