#ifndef FOREST_FUNCS_H
#define FOREST_FUNCS_H

#include <unordered_map>
#include "pool.hpp"

using namespace pooling;

struct tree_data
{
    tree_data(): volume(0), tree_size(1) {}
    
    tree_data(volume_t vol, counter_t size): volume(vol), tree_size(size) {}
    
    volume_t   volume;
    
    counter_t  tree_size;
};

class forest
{
    friend pool;

    static std::unordered_map<pool *, tree_data> forest_map;

    static void add_new_tree(pool *new_tree);
    
    static void rm_tree(pool *rmv_tree);
    
    static std::vector<std::pair<pool *, pool *>> linked_trees;

    static void add_tree_link(pool *f_tree, pool *s_tree);

    static counter_t find_tree_link(pool *tree);

    static counter_t find_tree_link(pool *f, pool *s);
};

#endif