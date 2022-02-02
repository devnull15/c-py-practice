
#include "TreeHelp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef enum trunk_link_t
{
    SPACING = 0,
    LEFT_BRANCH,
    RIGHT_BRANCH,
    HLINE_EXTENSION,
    VLINE_EXTENSION
} trunk_link_t;

static char *long_trunks[] = {
    "\x20\x20\x20\x20",
    "\u250C\u2500\u2500\u2500",
    "\u2514\u2500\u2500\u2500",
    "\u2500\u2500\u2500\u2500",
    "\x20\x20\x20\x20\u2502"
};

static char *short_trunks[] = {
    "\x20\x20\x20",
    NULL,
    NULL,
    NULL,
    "\x20\x20\x20\u2502"
};


typedef struct _TRUNK TRUNK;



static inline void render_trunk_link(trunk_link_t link, bool shorten)
{
    if (shorten)
    {
        assert(link == SPACING || link == VLINE_EXTENSION);
        printf("%s", short_trunks[link]);
    }
    else
    {
        printf("%s", long_trunks[link]);
    }
}

struct _TRUNK
{
    TRUNK *prev;
    trunk_link_t value;
};

static void
show_trunks(TRUNK *trunk)
{
    if (NULL == trunk) return;

    show_trunks(trunk->prev);
    render_trunk_link(trunk->value, (NULL != trunk->prev && trunk->prev->value == VLINE_EXTENSION));
}

static void
print_tree_nodes(Tree_node *node, TRUNK *prev, bool isLeft)
{
    if (NULL == node)
    {
        return;
    }

    TRUNK _trunk = {
        .prev = prev,
        .value = SPACING
    };
    //TODO: Get rid of this later
    TRUNK *trunk = &_trunk;
    if (NULL != prev && prev->value == VLINE_EXTENSION)
    {
        trunk->value = SPACING;
    }
    print_tree_nodes(node->left, trunk, true);
    trunk_link_t prev_link = SPACING;
    if (!prev) //We are the head so just start with DASH
    {
        trunk->value = HLINE_EXTENSION;
    }
    else if(isLeft)
    {
        trunk->value = LEFT_BRANCH;
        prev_link = VLINE_EXTENSION;
    }
    else
    {
        trunk->value = RIGHT_BRANCH;
        prev->value = prev_link;
    }

    show_trunks(trunk);
    printf("%d\n", node->data);

    if (prev)
    {
        prev->value = prev_link;
    }

    trunk->value = VLINE_EXTENSION;
    print_tree_nodes(node->right, trunk, false);

}

void
print_tree(Tree *tree)
{
    print_tree_nodes(tree->head, NULL, false);
}



