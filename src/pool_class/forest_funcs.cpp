#include <cassert>
#include "forest_funcs.hpp"

std::unordered_map <pool *, tree_data> forest::forest_map;

std::vector<std::pair<pool *, pool *>> forest::linked_trees;

void forest::add_new_tree(pool *new_tree)
{
    forest_map[new_tree] = tree_data();
}

void forest::rm_tree(pool *rmv_tree)
{
    forest_map.erase(rmv_tree);
}

void forest::add_tree_link(pool *f_tree, pool *s_tree)
{
    linked_trees.push_back(std::pair<pool *, pool *> (f_tree, s_tree));
}

counter_t forest::find_tree_link(pool *tree)
{
    for (counter_t i = 0; i < linked_trees.size(); i++)
        if (linked_trees[i].first == tree || linked_trees[i].second == tree)
            return i;
    
    return LINK_NOT_FOUND;
}

counter_t forest::find_tree_link(pool *f, pool *s)
{
    if (!f->check_for_side_links() || !s->check_for_side_links())
        return LINK_NOT_FOUND;

    for (counter_t i = 0; i < linked_trees.size(); i++)
    {
        if (linked_trees[i].first == f || linked_trees[i].first == s)
            if (linked_trees[i].second == f || linked_trees[i].second == s)
                return i;
    }
    
    return LINK_NOT_FOUND;
}