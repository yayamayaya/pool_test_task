#include "pool.hpp"

#include <cassert>

using namespace pooling;

std::unordered_map <pool *, pool_forest::pool_tree> pool_forest::forest_map;

// std::vector<pool *> pooling::active_pools;

// std::unordered_map<pool *, int> pooling::tree_sizes;

// static counter_t counter = 0;

/*pool_ind_t pool::return_this_index()
{
    if (children.size() != 0)
        return active_pools[children[0]]->parent;
    
    for (auto i = 0; i < active_pools.size(); i++)
        if (active_pools[i] == this)
            return i;

    _LOG[ERROR] << "> Allocated pool wasn't found, error" << END_;
    return -1;
}*/

/*pool_ind_t pool::root_index()
{
    pool_ind_t index = parent;
    while (index != root)
        index = active_pools[index]->parent;

    return return_this_index();
}*/

pool *pool::return_root()
{
    if (parent == nullptr)
        return this;

    pool *root = parent;
    while (root->parent != nullptr)
        root = root->parent;

    return root;
}

counter_t pool::get_tree_size()
{
    return pool_forest::return_tree_info(return_root()).tree_size;
}

void pool::add_water(volume_t vol)
{
    pool *root = return_root();

    pool_forest::pool_tree &tree_info = pool_forest::return_tree_info(root);

    tree_info.volume += vol / tree_info.tree_size;
}

volume_t pool::show_water_volume()
{
    pool *root = return_root();

    return pool_forest::return_tree_info(root).volume;
}

pool *pool::add_new_pool()
{
    pool *new_pool = new pool;

    pool_forest::add_new_tree(new_pool);

    return new_pool;
}

pool::~pool() 
{
    for (int i = 0; i < children.size(); i++)
        children[i]->~pool();

    // delete this;
}

void pool::delete_all_pools()
{
    _LOG << pool_forest::forest_map.size() << " <- size!!" << END_;

    for (auto it = pool_forest::forest_map.begin(); it != pool_forest::forest_map.end(); it++)
        delete it->first;

    pool_forest::forest_map.clear();
}

ret_t pool::connect_pool(pool *conn_pool)
{
    assert(conn_pool);

    auto it = pool_forest::forest_map.find(conn_pool);
    if (it != pool_forest::forest_map.end())
    {
        _LOG << "> tree root is found\n" << END_;

        return 0;
    }
}

void pool::delete_child(pool *ch)
{
    for (auto it = children.begin(); it < children.end(); it++)
        if (*it == ch)
            children.erase(it);
}

// ret_t pool::add_pool_connection(pool_ind_t conn_ind)
// {
//     if (root_index() == active_pools[conn_ind]->root_index())
//     {
//         side_link                          = true;
//         active_pools[conn_ind]->side_link  = true;

//         _LOG << "> Side bounding" << END_;

//         return 1;
//     }

//     if (parent == root)
//     {
//         parent = conn_ind;

//         // active_pools[conn_ind]->children.push_back();
//     }

//     if (parent == root || active_pools[conn_ind]->parent == root)
//     {
//         _LOG << "> root's bounding" << END_;

//         return 0;
//     }

//     _LOG << "> tree's bounding" << END_;
//     return 0;
// }