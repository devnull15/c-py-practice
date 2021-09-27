/*
    Reference the CUnit Users Guide:
    http://cunit.sourceforge.net/doc/writing_tests.html#tests

    Step 1. Create the test cases
    Step 2. _add the test suite to the registry
    Step 3. _add the test to the suite.
*/

#include "../include/Cllist.h"
#include <stdlib.h>
#include <CUnit/CUnitCI.h>
#include <CUnit/CUnit.h>

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

static void
print_circularly_llist_data(Circularly_llist *cllist)
{
    if (!cllist || !cllist->head)
        return;

    Circularly_llist_node *cur_node = cllist->head;
    Circularly_llist_node *last_node = cllist->tail;
    Circularly_llist_node *tmp_node = cur_node;

    printf("\t  {");

    int node_count = 0;
    while (tmp_node)
    {
        printf("%d", tmp_node->data);

        node_count++;
        if (tmp_node == last_node)
        {
            break;
        }
        if (tmp_node)
            printf(", ");
        tmp_node = tmp_node->next;
    }

    printf("}\n");
    printf("\t   ");
}

/* ------ 1. Test cases ------ */
static void test_circularly_llist_new(void)
{
    Circularly_llist *cllist = NULL;

    cllist = circularly_llist_new();
    CU_ASSERT_PTR_NOT_NULL(cllist);
    if (cllist)
        circularly_llist_destroy(cllist);
}

static void test_circularly_llist_add(void)
{
    Circularly_llist *cllist = circularly_llist_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(cllist);

    int total_nodes = rand_container_size();

    printf("_adding %d %s. ", total_nodes, total_nodes == 1 ? "node" : "nodes");
    for (int idx = 0; idx < total_nodes; idx++)
    {
        int newNum = rand_value();
        CU_ASSERT_TRUE(circularly_llist_add(cllist, newNum));
    }

    int node_count = 0;
    Circularly_llist_node *temp = cllist->head;
    Circularly_llist_node *last = cllist->tail;
    while (temp)
    {
        node_count++;
        if (temp == last)
            break;
        temp = temp->next;
    }

    printf("%d %s successfully added to linked list.\n", node_count, node_count == 1 ? "node" : "nodes");
    print_circularly_llist_data(cllist);
    printf("\t...%u vs. %u", total_nodes, node_count);
    CU_ASSERT_EQUAL(total_nodes, node_count);

    circularly_llist_destroy(cllist);
}

static void test_circularly_llist_is_circular(void)
{
    Circularly_llist *cllist = circularly_llist_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(cllist);

    int total_nodes = rand_container_size();

    printf("_adding %d %s. ", total_nodes, total_nodes == 1 ? "node" : "nodes");
    for (int idx = 0; idx < total_nodes; idx++)
    {
        int newNum = rand_value();
        CU_ASSERT_TRUE(circularly_llist_add(cllist, newNum));
    }

    int node_count = 0;
    Circularly_llist_node *temp = cllist->head;
    Circularly_llist_node *last = cllist->tail;
    while (temp)
    {
        node_count++;
        if (temp == last)
            break;
        temp = temp->next;
    }

    puts("");
    print_circularly_llist_data(cllist);

    bool addRet = circularly_llist_is_circular(cllist);
    printf("\t_is the list circular?\n");
    printf("\t...");
    CU_ASSERT_TRUE(addRet);

    circularly_llist_destroy(cllist);
}

static void
test_circularly_llist_add_to_null(void)
{
    Circularly_llist *cllist = NULL;

    int total_nodes = 1;
    bool addRet = false;

    printf("_adding %d %s to NULL list. ", total_nodes, total_nodes == 1 ? "node" : "nodes");
    addRet = circularly_llist_add(cllist, rand_value());

    printf("%s\n\t...", addRet ? "incorrectly returned True" : "successfully returned False");
    CU_ASSERT_FALSE(addRet);
}

static void
test_circularly_llist_find(void)
{
    // Build Linked list to test.
    int node_count = rand_container_size();
    int nodeTo_find = (rand() % (node_count - 1)) + 1;
    int intTo_find = 0;

    Circularly_llist *testList = circularly_llist_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(testList);

    printf("_adding %d test %s.\n", node_count, node_count == 1 ? "node" : "nodes");
    printf("\t_node to find: %d of %d\n", nodeTo_find, node_count);
    for (int idx = 0; idx < node_count; idx++)
    {
        int num = rand_value();
        circularly_llist_add(testList, num);

        if ((idx + 1) == nodeTo_find)
        {
            intTo_find = num;
        }
    }
    // End of test list build

    Circularly_llist_node *ret_node = NULL;
    print_circularly_llist_data(testList);
    printf("\tSearching for value %d\n", intTo_find);
    printf("\t...");

    ret_node = circularly_llist_find(testList, intTo_find);

    CU_ASSERT_PTR_NOT_NULL(ret_node);
    if (ret_node)
        CU_ASSERT_EQUAL(ret_node->data, intTo_find);

    circularly_llist_destroy(testList);
}

static void
test_circularly_llist_find_nothing(void)
{
    // Build Linked list to test.
    int node_count = rand_container_size();

    Circularly_llist *testList = circularly_llist_new();
    if (NULL == testList)
    {
        CU_FAIL_FATAL("ERROR: Memory Allocation Failed\n");
        return;
    }

    printf("_adding %d test %s.\n", node_count, node_count == 1 ? "node" : "nodes");
    for (int idx = 0; idx < node_count; idx++)
    {
        circularly_llist_add(testList, rand_value());
    }
    print_circularly_llist_data(testList);
    // End of test list build

    Circularly_llist_node *ret_node = NULL;
    printf("\tSearching for value %d\n\t...", 0);

    ret_node = circularly_llist_find(testList, 0);

    CU_ASSERT_PTR_NULL(ret_node);
    circularly_llist_destroy(testList);
}

static void
test_circularly_llist_find_no_list(void)
{
    Circularly_llist *testList = NULL;
    int value = rand_value();

    printf("_finding %d %s from NULL list\n\t...", value, "node");

    Circularly_llist_node *ret_node = NULL;

    ret_node = circularly_llist_find(testList, 0);

    CU_ASSERT_PTR_NULL(ret_node);
}

static void
test_circularly_llist_rm_head(void)
{

    int firstData = 0;
    // Build Linked list to test.
    int node_count = rand_container_size();

    Circularly_llist *testList = circularly_llist_new();

    printf("_adding %d test %s.\n", node_count, node_count == 1 ? "node" : "nodes");
    for (int idx = 0; idx < node_count; idx++)
    {
        int num = rand_value();
        circularly_llist_add(testList, num);

        if (0 == idx)
        {
            firstData = num;
        }
    }
    print_circularly_llist_data(testList);
    // End of test list build

    printf("\tRemoving head of cllist.\n");
    Circularly_llist_node *oldHead = circularly_llist_rm_head(testList);
    print_circularly_llist_data(testList);
    printf("\tChecking if still circular.\n");
    printf("\t...");

    CU_ASSERT_PTR_NOT_NULL(oldHead);
    if (oldHead)
        CU_ASSERT_EQUAL(oldHead->data, firstData);
    CU_ASSERT_PTR_NOT_NULL_FATAL(testList->head);
    CU_ASSERT_PTR_NOT_NULL_FATAL(testList->tail);
    CU_ASSERT_EQUAL(testList->tail->next, testList->head);

    if (oldHead)
        free(oldHead);

    circularly_llist_destroy(testList);
}

static void
test_circularly_llist_rm_head_single_node(void)
{

    Circularly_llist *testList = circularly_llist_new();

    printf("Generating test List with one node.\n");
    int firstData = rand_value();
    circularly_llist_add(testList, firstData);
    print_circularly_llist_data(testList);

    printf("\tRemoving head of cllist.\n");
    Circularly_llist_node *oldHead = circularly_llist_rm_head(testList);
    print_circularly_llist_data(testList);

    CU_ASSERT_PTR_NOT_NULL_FATAL(oldHead);
    CU_ASSERT_EQUAL(oldHead->data, firstData);
    CU_ASSERT_PTR_NULL(testList->head);
    CU_ASSERT_PTR_NULL(testList->tail);

    free(oldHead);
    circularly_llist_destroy(testList);
}

static void
test_circularly_llist_rm_head_from_null(void)
{

    Circularly_llist_node *oldHead = circularly_llist_rm_head(NULL);

    CU_ASSERT_PTR_NULL(oldHead);
}

static void
test_circularly_llist_rm_head_no_nodes(void)
{
    // Build Linked list to test.
    Circularly_llist *testList = circularly_llist_new();

    printf("No _nodes in cllist.\n");
    Circularly_llist_node *oldHead = circularly_llist_rm_head(testList);
    printf("\t...");

    CU_ASSERT_PTR_NULL(oldHead);

    circularly_llist_destroy(testList);
}

static void
test_circularly_llist_rm_tail(void)
{
    int firstData = 0;
    // Build Linked list to test.
    int node_count = rand_container_size();

    Circularly_llist *testList = circularly_llist_new();

    printf("_adding %d test %s.\n", node_count, node_count == 1 ? "node" : "nodes");
    for (int idx = 0; idx < node_count; idx++)
    {
        int num = rand_value();
        circularly_llist_add(testList, num);

        if (node_count - 1 == idx)
        {
            firstData = num;
        }
    }
    print_circularly_llist_data(testList);
    // End of test list build

    printf("\tRemoving tail of cllist.\n");
    Circularly_llist_node *oldTail = circularly_llist_rm_tail(testList);
    print_circularly_llist_data(testList);
    printf("\tChecking if still circular.\n");
    printf("\t...");

    CU_ASSERT_PTR_NOT_NULL(oldTail);
    if (oldTail)
        CU_ASSERT_EQUAL(oldTail->data, firstData);
    CU_ASSERT_EQUAL(testList->tail->next, testList->head);

    free(oldTail);

    circularly_llist_destroy(testList);
}

static void
test_circularly_llist_rm_tail_from_null(void)
{
    Circularly_llist *testList = NULL;

    printf("\n");
    Circularly_llist_node *oldTail = circularly_llist_rm_tail(testList);
    printf("\t...");

    CU_ASSERT_PTR_NULL(oldTail);
}

static void
test_circularly_llist_rm_tail_no_nodes(void)
{
    // Build Linked list to test.
    Circularly_llist *testList = circularly_llist_new();

    printf("No _nodes in cllist.\n");
    Circularly_llist_node *oldTail = circularly_llist_rm_tail(testList);
    printf("\t...");

    CU_ASSERT_PTR_NULL(oldTail);

    circularly_llist_destroy(testList);
}

static void
test_circularly_llist_rm(void)
{

    // Build Linked list to test.
    int node_count = rand_container_size();

    int nodeToRemove = (rand() % (node_count - 1)) + 1;
    int intToRemove = 0;

    Circularly_llist *testList = circularly_llist_new();
    if (NULL == testList)
    {
        CU_FAIL_FATAL("ERROR: Memory Allocation Failed\n");
        return;
    }

    printf("_adding %d test %s.\n", node_count, node_count == 1 ? "node" : "nodes");
    printf("\t_node to remove: %d of %d\n", nodeToRemove, node_count);
    for (int idx = 0; idx < node_count; idx++)
    {
        int num = rand_value();
        circularly_llist_add(testList, num);

        if ((idx + 1) == nodeToRemove)
        {
            intToRemove = num;
        }
    }
    print_circularly_llist_data(testList);
    // End of test list build

    Circularly_llist_node *ret_node = NULL;
    printf("\tRemoving value %d\n", intToRemove);

    ret_node = circularly_llist_rm(testList, intToRemove);
    print_circularly_llist_data(testList);

    CU_ASSERT_PTR_NOT_NULL_FATAL(ret_node);
    CU_ASSERT_EQUAL(ret_node->data, intToRemove);

    //Check to see if node count has been adjusted
    int new_node_count = 0;
    Circularly_llist_node *temp_node = testList->head;
    Circularly_llist_node *last_node = testList->tail;
    while (temp_node)
    {
        new_node_count++;
        if (temp_node == last_node)
            break;
        temp_node = temp_node->next;
    }

    printf("\t%d %s before removal. %d %s after removal\n",
           node_count,
           node_count == 1 ? "node" : "nodes",
           new_node_count,
           new_node_count == 1 ? "node" : "nodes");

    printf("\t...");
    CU_ASSERT_EQUAL(node_count - 1, new_node_count);

    circularly_llist_destroy(testList);

    free(ret_node);
}

static void
test_circularly_llist_destroy(void)
{
    // Build Linked list to test.
    int node_count = rand_container_size();

    Circularly_llist *testList = circularly_llist_new();
    if (NULL == testList)
    {
        CU_FAIL_FATAL("ERROR: Memory Allocation Failed\n");
        return;
    }

    printf("_adding %d test %s.\n", node_count, node_count == 1 ? "node" : "nodes");
    for (int idx = 0; idx < node_count; idx++)
    {
        circularly_llist_add(testList, rand_value());
    }
    print_circularly_llist_data(testList);
    // End of test list build

    bool retVal = false;
    printf("\t_destroying _circular Linked List\n");

    CU_ASSERT_PTR_NOT_NULL(testList);
    if (testList)
    {
        CU_ASSERT_PTR_NOT_NULL_FATAL(testList->head);
        CU_ASSERT_PTR_NOT_NULL_FATAL(testList->head->next);
    }

    retVal = circularly_llist_destroy(testList);
    CU_ASSERT_TRUE(retVal);

    printf("\t...");
}
