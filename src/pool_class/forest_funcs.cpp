#include "pool.hpp"
#include <cassert>

using namespace pooling;

pool_forest::pool_tree &pool_forest::return_tree_info(pool *root)
{
    if (root->parent)
        return return_tree_info(root->parent);

    auto it = forest_map.find(root);
    if (it != forest_map.end())
        return forest_map[root];

    throw "tree is not in the forest";
}

void pool_forest::add_new_tree(pool *new_tree)
{
    forest_map[new_tree] = pool_tree();
}

void pool_forest::rm_tree(pool *rmv_tree)
{
    forest_map.erase(rmv_tree);
}

pool *pool_forest::convert_tree_to_a_subtree(pool *tree_node)
{
    assert(tree_node);

    pool *node_parent = nullptr;
    pool *prev_node   = nullptr;
    pool *curr_node   = tree_node;

    while(1)
    {
        node_parent = curr_node->parent;
        curr_node->parent = prev_node;

        if (prev_node)
            curr_node->delete_child(prev_node);
        
        if (!node_parent)
            break;

        curr_node->children.push_back(node_parent);
        
        prev_node = curr_node;
        curr_node = node_parent;
    }

    rm_tree(curr_node);

    return tree_node;
}

