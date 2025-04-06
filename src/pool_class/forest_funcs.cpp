#include "pool.hpp"

using namespace pooling;

pool_forest::pool_tree &pool_forest::return_tree_info(pool *root)
{
    return forest_map[root];
}

void pool_forest::add_new_tree(pool *new_tree)
{
    forest_map[new_tree] = pool_tree();
}

pool *pool_forest::convert_tree_to_a_subtree(pool *tree_node)
{
    pool *node_parent = nullptr;
    pool *prev_node = nullptr;

    while(1)
    {
        node_parent = tree_node->parent;
        tree_node->parent = prev_node;

        //rm prev_node from vector
        if (prev_node)
            tree_node->delete_child(prev_node);

        if (node_parent == nullptr)
            break;

        tree_node->children.push_back(node_parent);
        
        prev_node = tree_node;
        tree_node = node_parent;
    }
}

