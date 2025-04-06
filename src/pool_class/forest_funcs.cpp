#include "pool.hpp"
#include <cassert>

using namespace pooling;

pool::forest::tree_data &pool::forest::return_tree_info(pool *root)
{
    if (root->parent)
        return return_tree_info(root->parent);

    auto it = forest_map.find(root);
    if (it != forest_map.end())
        return forest_map[root];

    throw "tree is not in the forest";
}

void pool::forest::add_new_tree(pool *new_tree)
{
    forest_map[new_tree] = tree_data();
}

void pool::forest::rm_tree(pool *rmv_tree)
{
    forest_map.erase(rmv_tree);
}

pool *pool::forest::convert_tree_to_a_subtree(pool *tree_node)
{
    assert(tree_node);

    pool *node_parent = nullptr;
    pool *prev_node   = nullptr;
    pool *curr_node   = tree_node;

    while (1)
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

void pool::forest::add_tree_link(pool *f_tree, pool *s_tree)
{
    linked_trees.push_back(std::pair<pool *, pool *> (f_tree, s_tree));
}

counter_t pool::forest::find_tree_link(pool *tree)
{
    for (counter_t i = 0; i < linked_trees.size(); i++)
        if (linked_trees[i].first == tree || linked_trees[i].second == tree)
            return i;
    
    return LINK_NOT_FOUND;
}

counter_t pool::forest::find_tree_link(pool *f, pool *s)
{
    if (!f->side_link || !s->side_link)
        return LINK_NOT_FOUND;

    for (counter_t i = 0; i < linked_trees.size(); i++)
    {
        if (linked_trees[i].first == f || linked_trees[i].first == s)
            if (linked_trees[i].second == f || linked_trees[i].second == s)
                return i;
    }
    
    return LINK_NOT_FOUND;
}