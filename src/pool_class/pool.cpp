#include "pool.hpp"

#include <cassert>

using namespace pooling;

std::unordered_map <pool *, pool_forest::pool_tree> pool_forest::forest_map;

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
    try
    {
        return pool_forest::return_tree_info(this).tree_size;
    }
    catch(const char *msg)
    {
        _LOG[ERROR] << "Exception throw: " << msg << END_;
        return -1;
    }
}

volume_t pool::show_water_volume()
{
    try
    {
        return pool_forest::return_tree_info(this).volume;
    }
    catch(const char *msg)
    {
        _LOG[ERROR] << "Exception throw: " << msg << END_;
        return -1;
    }
}

void pool::add_water(volume_t vol)
{
    try
    {
        pool_forest::pool_tree &tree_info = pool_forest::return_tree_info(this);
        tree_info.volume += vol / tree_info.tree_size;
    }
    catch(const char *msg)
    {
        _LOG[ERROR] << "Exception throw: " << msg << END_;
    }
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
        delete children[i];
}

void pool::delete_all_pools()
{
    // _LOG << pool_forest::forest_map.size() << " <- size!!" << END_;

    for (auto it = pool_forest::forest_map.begin(); it != pool_forest::forest_map.end(); it++)
        delete it->first;

    pool_forest::forest_map.clear();
}

ret_t pool::connect_pool(pool *conn_pool)
{
    assert(conn_pool);

    if (this->return_root() == conn_pool->return_root())
    {
        this->side_link = 1;
        conn_pool->side_link = 1;

        return 0;
    }

    try
    {
        pool_forest::pool_tree conn_info = pool_forest::return_tree_info(conn_pool);
        pool_forest::pool_tree &data       = pool_forest::return_tree_info(this);

        counter_t this_size = data.tree_size;
        data.tree_size += conn_info.tree_size;
    
        data.volume = (data.volume * this_size + conn_info.volume * conn_info.tree_size) / data.tree_size;
    }
    catch(const char *msg)
    {
        _LOG[ERROR] << "Exception throw: " << msg << END_;
        return EXCEPTION_THROW;
    }

    pool *transformed_pool = pool_forest::convert_tree_to_a_subtree(conn_pool);
    transformed_pool->parent = this;
    children.push_back(transformed_pool);

    return 0;
}

void pool::delete_child(pool *ch)
{
    for (auto it = children.begin(); it < children.end(); it++)
        if (*it == ch)
            children.erase(it);
}