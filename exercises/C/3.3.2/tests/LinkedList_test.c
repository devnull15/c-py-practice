/*
    Reference the CUnit Users Guide:
    http://cunit.sourceforge.net/doc/writing_tests.html#tests

    Step 1. Create the test cases
    Step 2. _add the test suite to the registry
    Step 3. _add the test to the suite.
*/

#include "../include/LinkedList.h"
#include <stdlib.h>
#include <CUnit/CUnitCI.h>
#include <CUnit/CUnit.h>
#include "../Common/test_wrap_assert/test_wrap_assert.h"

#define MIN_LIST_SIZE 5
#define MAX_LIST_SIZE 20
#define MIN_RAND 1
#define MAX_RAND 100
/**
 *
 *   Various loops in this test suite attemp to randomly generate a unique value
 *   Not already in the data structure. There is an edge case where the user
 *   does not implement the correct return value for the compare function. This
 *   will cause the test suite to loop indefinitely, or nearly so. Having a max
 *   number of attempts for the generation of a single unique value will give a
 *   gaurantee of halting. With a high enough number there is almost a certainty
 *   that a unique nubmer is generated.
 */
#define MAX_PATIENCE 1000

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

/**
 * @brief Helper function to print a linked list of integer values to stdout
 *
 * @param list a pointer to an allocated linked list object
 */
static void print_linked_list_integer_data(linked_list_t *list)
{
    if (!list)
        return;

    printf("\t  {");

    for (size_t i = 0; i < linked_list_size(list); i++)
    {
        void *data = linked_list_at(list, i);
        printf("%d", *(int *)data);

        printf(", ");
    }

    printf("}\n");
}

/**
 * @brief Helper function intended as the cmp_f cmp parameter to linked_list_new()
 *
 * @param lhs a pointer to the left-hand-side value being compoared
 * @param rhs a pointer to the right-hand-side value being compared
 * @return int 0 if lhs == rhs, 1 if lhs > rhs, -1 if lhs < rhs
 */
int cmp_int(const void *lhs, const void *rhs)
{
    if (*(int *)lhs > *(int *)rhs)
    {
        return 1;
    }
    if (*(int *)lhs == *(int *)rhs)
    {
        return 0;
    }
    // lhs < rhs
    return -1;
}

/* ------ 1. Test cases ------ */
/**
 * @brief Verify that the pointer returned from linked_list_new
 *        is not NULL.
 *
 */
static void test_linked_list_new(void)
{
    CU_CATCH_ASSERTIONS();
    linked_list_t *list = NULL;
    list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL(list);
    linked_list_delete(&list);
}

/**
 * @brief Verify that the linked_list_size() function works correctly
 *
 */
static void test_linked_list_size(void)
{
    CU_CATCH_ASSERTIONS();
    linked_list_t *list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);

    int total_nodes = rand_container_size();

    printf("_adding %d %s. ", total_nodes, total_nodes == 1 ? "node" : "nodes");
    for (int idx = 0; idx < total_nodes; idx++)
    {
        void *data = malloc(sizeof(int));
        *(int *)data = rand_value();
        size_t previous_size = linked_list_size(list);
        linked_list_append(list, data);
        CU_ASSERT_TRUE(linked_list_size(list) > previous_size);
    }
    printf("%s", "\n\tVerifying size is equal to nodes added.\n\t...");
    CU_ASSERT_EQUAL(linked_list_size(list), total_nodes);

    linked_list_delete(&list);
}

/**
 * @brief Verify that linked_list_append() operates correctly
 *
 * Verifies the pointer returned from linked_list_new is
 * not NULL, then verifies that linked_list_size() reports
 * a greater size after each call to linked_list_append(),
 * and finally verifies the final node count in the linked
 * list is equal to the total nodes that should have been
 * added.
 *
 */
static void test_linked_list_append(void)
{
    CU_CATCH_ASSERTIONS();
    linked_list_t *list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);

    int total_nodes = rand_container_size();

    printf("_adding %d %s. ", total_nodes, total_nodes == 1 ? "node" : "nodes");
    for (int idx = 0; idx < total_nodes; idx++)
    {
        void *data = malloc(sizeof(int));
        *(int *)data = rand_value();
        size_t previous_size = linked_list_size(list);
        linked_list_append(list, data);
        CU_ASSERT_TRUE(linked_list_size(list) > previous_size);
    }

    size_t node_count = linked_list_size(list);

    printf("%ld %s successfully added to linked list.\n", node_count, node_count == 1 ? "node" : "nodes");
    print_linked_list_integer_data(list);
    printf("\t...");
    CU_ASSERT_EQUAL(total_nodes, node_count);

    linked_list_delete(&list);
}

/**
 * @brief Verify that appending to a NULL list fails an assertion check
 *
 */
static void test_linked_list_append_to_null(void)
{

    void *data = malloc(sizeof(int));
    *(int *)data = rand_value();

    printf("_adding 1 node to NULL list. \n\t...");
    CU_ASSERT_ASSERTED(linked_list_append(NULL, data));
    printf("Here\n");
    free(data);
}

/**
 * @brief Verify that linked_list_find_first_after() and linked_list_at()
 *        are functioning properly.
 *
 */
static void test_linked_list_find_with_valid_value(void)
{
    CU_CATCH_ASSERTIONS();
    // Build Linked list to test.
    size_t node_count = rand_container_size();
    size_t node_to_find = (rand() % (node_count - 1)) + 1;
    int int_to_find = 0;

    linked_list_t *test_list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_list);

    printf("_adding %ld test %s.\n", node_count, node_count == 1 ? "node" : "nodes");
    printf("\tNode to find: %ld of %ld\n", node_to_find, node_count);
    for (size_t idx = 0; idx < node_count; idx++)
    {
        void *data = malloc(sizeof(int));
        // Loop until the random generated value doesn't exist in the list
        size_t patience = MAX_PATIENCE;
        while (true)
        {
            *(int *)data = rand_value();
            if ((size_t)-1 == linked_list_find_first_after(test_list, data, 0))
            {
                break;
            }
            CU_ASSERT_NOT_EQUAL_FATAL(patience--, 0);
        }
        linked_list_append(test_list, data);

        if ((idx + 1) == node_to_find)
        {
            int_to_find = *(int *)data;
        }
    }
    // End of test list build

    print_linked_list_integer_data(test_list);
    printf("\tSearching for value %d\n", int_to_find);
    printf("\t...");

    size_t found_index = linked_list_find_first_after(test_list, &int_to_find, 0);

    // ExamHeader should specify the function returns (size_t) -1 when a value was NOT found
    CU_ASSERT_PTR_NOT_EQUAL((size_t)-1, found_index);
    if (found_index != (size_t)-1)
    {
        void *data = linked_list_at(test_list, found_index);
        if (NULL != data)
        {
            CU_ASSERT_EQUAL(*(int *)data, int_to_find);
        }
        else
        {
            CU_FAIL("Value was found, but linked_list_at() returned a NULL data pointer");
        }
    }
    linked_list_delete(&test_list);
}

/**
 * @brief Verify that linked_list_find_first_after() doesn't find nodes
 *        that shouldn't exist.
 *
 */
static void test_linked_list_find_with_invalid_value(void)
{
    CU_CATCH_ASSERTIONS();
    // Build Linked list to test.
    size_t node_count = rand_container_size();

    linked_list_t *test_list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_list);

    printf("_adding %zu test %s.\n", node_count, node_count == 1 ? "node" : "nodes");
    for (int idx = 0; idx < node_count; idx++)
    {
        void *data = malloc(sizeof(int));
        *(int *)data = rand_value();
        linked_list_append(test_list, data);
    }
    print_linked_list_integer_data(test_list);
    // End of test list build

    void *missing_value = malloc(sizeof(int));
    *(int *)missing_value = 0;
    int patience = MAX_PATIENCE;
    while (0 == *(int *)missing_value)
    {
        CU_ASSERT_NOT_EQUAL_FATAL(patience--, 0);
        int temp = rand_value();
        // (size_t)-1 is returned when the item was NOT found
        if ((size_t)-1 == linked_list_find_first_after(test_list, &temp, 0))
        {
            *(int *)missing_value = temp;
        }
    }

    printf("\tSearching for value %d\n\t...", *(int *)missing_value);

    // ExamHeader should state that linked_list_find_first_after() returns (size_t)-1 if a value was NOT found
    CU_ASSERT_EQUAL(linked_list_find_first_after(test_list, missing_value, 0), (size_t)-1);

    free(missing_value);
    linked_list_delete(&test_list);
}

/**
 * @brief Verify that linked_list_find_first_after() fails an assertion check
 *        when the list parameter is NULL.
 *
 */
static void test_linked_list_find_Null_list(void)
{
    linked_list_t *test_list = NULL;
    int value = rand_value();

    printf("_finding %d %s from NULL list\n\t...", value, "node");

    CU_ASSERT_ASSERTED(linked_list_find_first_after(test_list, &value, 0));
}

/**
 * @brief Verify that linked_list_at() function fails an assertion check when
 *        the index parameter is invalid.
 *
 */
static void test_linked_list_at_invalid_index(void)
{
    CU_CATCH_ASSERTIONS();
    // Build Linked list to test.
    size_t node_count = rand_container_size();
    size_t node_to_find = (rand() % (node_count - 1)) + 1;
    int int_to_find = 0;

    linked_list_t *test_list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_list);

    printf("_adding %ld test %s.\n", node_count, node_count == 1 ? "node" : "nodes");
    printf("\tNode to find: %ld of %ld\n", node_to_find, node_count);
    for (size_t idx = 0; idx < node_count; idx++)
    {
        void *data = malloc(sizeof(int));
        *(int *)data = rand_value();
        linked_list_append(test_list, data);

        if ((idx + 1) == node_to_find)
        {
            int_to_find = *(int *)data;
        }
    }
    // End of test list build

    print_linked_list_integer_data(test_list);
    printf("\tSearching for value %d\n", int_to_find);
    printf("\t...");

    size_t found_index = linked_list_find_first_after(test_list, &int_to_find, 0);

    // ExamHeader should specify the function returns (size_t) -1 when a value was NOT found
    CU_ASSERT_PTR_NOT_EQUAL((size_t)-1, found_index);
    if (found_index != (size_t)-1)
    {
        // INVALID index test here
        void *data;
        size_t current_size = linked_list_size(test_list);
        printf("\tLinked list size is %ld, Searching for index %ld\n", current_size, current_size + rand_value());
        CU_PAUSE_CATCHING_ASSERTIONS();
        CU_ASSERT_ASSERTED(data = linked_list_at(test_list, linked_list_size(test_list) + 1));
        CU_CONTINUE_CATCHING_ASSERTIONS();
    }

    linked_list_delete(&test_list);
}

/**
 * @brief Verify linked_list_del_at() is functioning properly by removing
 *        the head node. Proper deallocation of memory for the node will
 *        be checked by the valgrind pipeline that follows this test suite.
 *
 */
static void test_linked_list_del_head(void)
{
    CU_CATCH_ASSERTIONS();
    int first_data = 0;
    // Build Linked list to test.
    size_t node_count = rand_container_size();

    linked_list_t *test_list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_list);

    printf("_adding %ld test %s.\n", node_count, node_count == 1 ? "node" : "nodes");
    for (size_t idx = 0; idx < node_count; idx++)
    {
        void *data = malloc(sizeof(int));
        *(int *)data = rand_value();
        linked_list_append(test_list, data);

        if (0 == idx)
        {
            first_data = *(int *)data;
        }
    }
    print_linked_list_integer_data(test_list);
    // End of test list build

    int old_head_data = *(int *)linked_list_at(test_list, 0);
    printf("\tRemoving head of linked list.\n");
    size_t prev_size = linked_list_size(test_list);
    linked_list_del_at(test_list, 0);
    size_t new_size = linked_list_size(test_list);

    print_linked_list_integer_data(test_list);
    printf("\t...");

    // Verify that linked_list_del_at() decrements the size of the linked list
    CU_ASSERT_TRUE(prev_size == (new_size + 1));

    // Verify that index 0 of the linked list is actually the head being removed
    CU_ASSERT_EQUAL(old_head_data, first_data);

    linked_list_delete(&test_list);
}

/**
 * @brief Verify that linked_list_del_at() fails an assertion check
 *        when passed a NULL list pointer.
 *
 */
static void test_linked_list_del_head_from_null_list(void)
{
    CU_ASSERT_ASSERTED(linked_list_del_at(NULL, 0));
}

/**
 * @brief Helper function as part of test_linked_list_for_each.
 *
 * @param item A pointer to a data value in a node.
 *
 * @param ctx a pointer to the second parameter defined by the action function.
 *            (The first parameter to action is the data pointer in a list node.)
 *            An action function that doesn't require the second parameter can
 *            simply set this to NULL.
 */
void mod_by_2(void *item, void *ctx)
{
    *((int *)item) = (*((int *)item) % 2 + 2) % 2;
};

/**
 * @brief Helper function as part of test_linked_list_for_each.
 *
 * @param item A pointer to a data value in a node.
 *
 * @param ctx a pointer to the second parameter defined by the action function.
 *            (The first parameter to action is the data pointer in a list node.)
 *            An action function that doesn't require the second parameter can
 *            simply set this to NULL.
 */
void mod_by_n(void *item, void *ctx)
{
    int *n = (int *)ctx;
    *((int *)item) = (*((int *)item) % *n + *n) % *n;
}

/**
 * @brief Verify that the function insert before functions correctly.
 */
static void test_linked_list_insert_before(void)
{
    CU_CATCH_ASSERTIONS();

    // Asserts that the list pointer is not NULL.
    CU_PAUSE_CATCHING_ASSERTIONS();
    CU_ASSERT_ASSERTED(linked_list_insert_before(NULL, 0, NULL));
    CU_CONTINUE_CATCHING_ASSERTIONS();

    linked_list_t *list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);
    int total_nodes = rand_container_size();

    printf("_adding %d %s. ", total_nodes, total_nodes == 1 ? "node" : "nodes");
    for (int idx = 0; idx < total_nodes; idx++)
    {
        void *data = malloc(sizeof(int));
        *(int *)data = rand_value();
        size_t previous_size = linked_list_size(list);
        linked_list_append(list, data);
        CU_ASSERT_TRUE(linked_list_size(list) > previous_size);
    }

    size_t node_count = linked_list_size(list);

    printf("%ld %s successfully added to linked list.\n", node_count, node_count == 1 ? "node" : "nodes");
    print_linked_list_integer_data(list);
    printf("\t...");
    CU_ASSERT_EQUAL(total_nodes, node_count);

    // Asserts that index <= list size. If index == size, the element will be appended.
    CU_PAUSE_CATCHING_ASSERTIONS();
    CU_ASSERT_ASSERTED(linked_list_insert_before(list, linked_list_size(list) + 1, NULL));
    CU_CONTINUE_CATCHING_ASSERTIONS();

    //Select a random insertion point
    size_t node_to_insertbefore = (rand() % (linked_list_size(list) - 1));

    //Generate some random data.

    //Look for data in node before insertion.
    void *data = malloc(sizeof(int));
    *(int *)data = rand_value();
    printf("Inserting before node %lu with %u\n", node_to_insertbefore, *(int *)data);
    linked_list_insert_before(list, node_to_insertbefore, data);
    printf("After insert.\n");
    print_linked_list_integer_data(list);
    CU_ASSERT_EQUAL(node_to_insertbefore, linked_list_find_first_after(list, data, node_to_insertbefore));
    CU_ASSERT_EQUAL(node_count + 1, linked_list_size(list));

    linked_list_delete(&list);
}

/**
 * @brief Verify that the linked_list_for_each has been implemented and
 *        that the function called is performed on each node in the list.
 */
static void test_linked_list_for_each(void)
{
    CU_CATCH_ASSERTIONS();

    //Need to wrap the assert
    CU_PAUSE_CATCHING_ASSERTIONS();
    CU_ASSERT_ASSERTED(linked_list_for_each(NULL, (action_f)mod_by_2, NULL));
    CU_CONTINUE_CATCHING_ASSERTIONS();

    linked_list_t *list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);

    int total_nodes = rand_container_size();

    printf("_adding %d %s. ", total_nodes, total_nodes == 1 ? "node" : "nodes");
    for (int idx = 0; idx < total_nodes; idx++)
    {
        void *data = malloc(sizeof(int));
        *(int *)data = rand_value();
        size_t previous_size = linked_list_size(list);
        linked_list_append(list, data);
        CU_ASSERT_TRUE(linked_list_size(list) > previous_size);
    }

    size_t node_count = linked_list_size(list);

    printf("%ld %s successfully added to linked list.\n", node_count, node_count == 1 ? "node" : "nodes");
    print_linked_list_integer_data(list);
    printf("\t...");
    CU_ASSERT_EQUAL(total_nodes, node_count);

    linked_list_for_each(list, mod_by_2, NULL);
    ll_node_t *current = list->head;
    for (int idx = 0; idx < total_nodes; idx++)
    {
        if (*((int *)current->data) / 2 != 0)
        {
            CU_FAIL("Function not appplied.\n");
        }
    }

    linked_list_delete(&list);

    list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);
    //New list. Checking for using context.
    total_nodes = rand_container_size();

    printf("_adding %d %s. ", total_nodes, total_nodes == 1 ? "node" : "nodes");
    for (int idx = 0; idx < total_nodes; idx++)
    {
        void *data = malloc(sizeof(int));
        *(int *)data = rand_value();
        size_t previous_size = linked_list_size(list);
        linked_list_append(list, data);
        CU_ASSERT_TRUE(linked_list_size(list) > previous_size);
    }

    node_count = linked_list_size(list);

    printf("%ld %s successfully added to linked list.\n", node_count, node_count == 1 ? "node" : "nodes");
    print_linked_list_integer_data(list);
    printf("\t...");
    CU_ASSERT_EQUAL(total_nodes, node_count);

    int *number = malloc(sizeof(int));
    *(int *)number = rand_value();
    linked_list_for_each(list, mod_by_n, number);
    current = list->head;
    for (int idx = 0; idx < total_nodes; idx++)
    {
        if (*((int *)current->data) / *number != 0)
        {
            CU_FAIL("Function not appplied.\n");
        }
    }
    free(number);

    linked_list_delete(&list);
}

/**
 * @brief Verify that deleting a node from an empty list
 *        fails an assertion check.
 *
 */
static void test_linked_list_del_head_from_empty_list(void)
{
    CU_CATCH_ASSERTIONS();
    // Build Linked list to test.
    linked_list_t *test_list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_list);

    CU_PAUSE_CATCHING_ASSERTIONS();
    CU_ASSERT_ASSERTED(linked_list_del_at(test_list, 0));
    CU_CONTINUE_CATCHING_ASSERTIONS();
    linked_list_delete(&test_list);
}

static void test_linked_list_del_at_valid_index(void)
{
    CU_CATCH_ASSERTIONS();
    // Build Linked list to test.

    size_t node_count = rand_container_size();

    size_t node_to_remove = rand_range(1, node_count);
    int int_to_remove = 0;

    linked_list_t *test_list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_list);

    printf("_adding %ld test %s.\n", node_count, node_count == 1 ? "node" : "nodes");
    printf("\tNode to remove: %ld of %ld\n", node_to_remove, node_count);
    for (size_t idx = 0; idx < node_count; idx++)
    {
        void *data = malloc(sizeof(int));
        // generate_unique_value(&test_list, &data);
        size_t patience = MAX_PATIENCE;
        while (true)
        {
            *(int *)data = rand_value();
            if ((size_t)-1 == linked_list_find_first_after(test_list, data, 0))
            {
                break;
            }
            CU_ASSERT_NOT_EQUAL_FATAL(patience--, 0);
        }
        linked_list_append(test_list, data);

        if ((idx + 1) == node_to_remove)
        {
            int_to_remove = *(int *)data;
        }
    }
    print_linked_list_integer_data(test_list);
    // End of test list build

    printf("\tRemoving value %d\n", int_to_remove);

    size_t found_index = linked_list_find_first_after(test_list, &int_to_remove, 0);
    printf("\tfound_index = %zu\n", found_index);

    // ExamHeader should state that linked_list_find_first_after() returns (size_t)-1 when a value was NOT found
    CU_ASSERT_PTR_NOT_EQUAL(found_index, (size_t)-1);

    int found_data = -1;
    if (found_index != (size_t)-1)
    {
        found_data = *(int *)linked_list_at(test_list, found_index);
    }
    linked_list_del_at(test_list, found_index);
    print_linked_list_integer_data(test_list);

    // Confirm that linked_list_find_first_after is functioning properly because it found the right value
    CU_ASSERT_EQUAL(found_data, int_to_remove);

    //Check to see if node count has been adjusted
    size_t new_node_count = linked_list_size(test_list);

    printf("\t%ld %s before removal. %ld %s after removal\n",
           node_count,
           node_count == 1 ? "node" : "nodes",
           new_node_count,
           new_node_count == 1 ? "node" : "nodes");

    printf("\t...");
    CU_ASSERT_EQUAL(node_count - 1, new_node_count);

    linked_list_delete(&test_list);
}

static void test_linked_list_del_at_invalid_index(void)
{
    CU_CATCH_ASSERTIONS();
    // Build Linked list to test.
    size_t node_count = rand_container_size();

    size_t node_to_remove = (rand() % (node_count - 1)) + 1;
    int int_to_remove = 0;

    linked_list_t *test_list = NULL;
    test_list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_list);

    printf("_adding %ld test %s.\n", node_count, node_count == 1 ? "node" : "nodes");
    printf("\tNode to remove: %ld of %ld\n", node_to_remove, node_count);
    for (size_t idx = 0; idx < node_count; idx++)
    {
        void *data = malloc(sizeof(int));
        *(int *)data = rand_value();
        linked_list_append(test_list, data);
    }
    print_linked_list_integer_data(test_list);
    // End of test list build

    size_t invalid_index = linked_list_size(test_list) + 1;
    CU_PAUSE_CATCHING_ASSERTIONS();
    CU_ASSERT_ASSERTED(linked_list_del_at(test_list, invalid_index));
    CU_CONTINUE_CATCHING_ASSERTIONS();
    printf("\tFunction correctly throws assertion error on invalid index.\n");

    linked_list_delete(&test_list);
}

static void test_linked_list_delete(void)
{
    CU_CATCH_ASSERTIONS();
    // Build Linked list to test.
    size_t node_count = rand_container_size();

    linked_list_t *test_list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_list);

    printf("_adding %ld test %s.\n", node_count, node_count == 1 ? "node" : "nodes");
    for (int idx = 0; idx < node_count; idx++)
    {
        void *data = malloc(sizeof(int));
        *(int *)data = rand_value();
        linked_list_append(test_list, data);
    }
    print_linked_list_integer_data(test_list);
    // End of test list build

    bool retVal = false;
    printf("\t_destroying Linked List\n");

    // Make sure the list pointer is NOT NULL
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_list);

    // Make sure the list is NOT empty
    CU_ASSERT_TRUE(linked_list_size(test_list) != 0);

    linked_list_delete(&test_list);
    //    CU_ASSERT_TRUE(retVal, C_NONE, 3, "3.3.1b", "3.3.2g", "3.3.2h");

    printf("\t...");
}