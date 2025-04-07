#include "pool.hpp"
#include <cassert>

using namespace pooling;

std::unordered_map <pool *, forest::tree_data> forest::forest_map;

std::vector<std::pair<pool *, pool *>> forest::linked_trees;

counter_t pool::children_number()
{
    return children.size();
}

pool *pool::return_root()
{
    if (parent == nullptr)
        return this;

    pool *root = parent;
    while (root->parent != nullptr)
        root = root->parent;

    return root;
}

counter_t pool::count_subtree_size()
{
    counter_t cnt = 1;

    for (int i = 0; i < children.size(); i++)
        cnt += children[i]->count_subtree_size();
    
    return cnt;
}

volume_t pool::show_water_volume()
{
    try
    {
        return forest::return_tree_info(this).volume;
    }
    catch(...)
    {
        _LOG << "Exception throw, couldn't find water volume data for node: " << this << END_;
        return NO_WATER_FOUND;
    }
}

void pool::add_water(volume_t vol)
{
    try
    {
        forest::tree_data &tree_info = forest::return_tree_info(this);
        tree_info.volume += vol / tree_info.tree_size;
    }
    catch(...)
    {
        _LOG << "Exception throw, couldn't add water for node: " << this << END_;
    }
}

pool *pool::create_new_pool()
{
    pool *new_pool = new pool;

    forest::add_new_tree(new_pool);

    return new_pool;
}

pool::~pool() 
{
    for (int i = 0; i < children.size(); i++)
        delete children[i];
}

void pool::delete_all_pools()
{
    for (auto it = forest::forest_map.begin(); it != forest::forest_map.end(); it++)
        delete it->first;

    forest::forest_map.clear();
    forest::linked_trees.clear();
}

ret_t pool::connect_pool(pool *conn_pool)
{
    assert(conn_pool);
    if (conn_pool == this) return 0;

    if (this->return_root() == conn_pool->return_root())
    {
        if (check_for_connection(conn_pool))
            return ALREADY_CONNECT;

        this->side_link++;
        conn_pool->side_link++;

        forest::add_tree_link(this, conn_pool);

        return 0;
    }

    try
    {
        forest::tree_data conn_info   = forest::return_tree_info(conn_pool);
        forest::tree_data &data       = forest::return_tree_info(this);

        counter_t this_size = data.tree_size;
        data.tree_size += conn_info.tree_size;
    
        data.volume = (data.volume * this_size + conn_info.volume * conn_info.tree_size) / data.tree_size;
    }
    catch(...)
    {
        _LOG << "Exception throw, couldn't connect " << conn_pool << " to " << this << END_;
        return EXCEPTION_THROW;
    }

    pool *transformed_pool = forest::convert_tree_to_a_subtree(conn_pool);
    transformed_pool->parent = this;
    children.push_back(transformed_pool);

    return 0;
}

ret_t pool::disconnect_pool(pool_ind_t child_ind)
{
    return disconnect_pool(children[child_ind]);
}

ret_t pool::disconnect_pool(pool *dis_pool)
{
    assert(dis_pool);
    if (dis_pool == this) return 0;
    
    if (dis_pool == this->parent)
        return dis_pool->disconnect_pool(this);
        
    if (delete_child(dis_pool))
    {        
        counter_t link_ind = forest::find_tree_link(this, dis_pool);
        if (link_ind == LINK_NOT_FOUND) 
            return NO_CONNECTION;

        forest::linked_trees.erase(forest::linked_trees.begin() + link_ind);

        return 0;   
    }
    
    if (dis_pool->side_link)
        return reconnect_to_side_link(dis_pool);

    counter_t subtree_size = dis_pool->count_subtree_size();

    try
    {
        forest::tree_data &data       = forest::return_tree_info(this);

        data.tree_size -= subtree_size;

        forest::forest_map[dis_pool] = {data.volume, subtree_size};

        dis_pool->parent = nullptr;

        return 0;
    }
    catch(...)
    {
        _LOG << "Exception throw, couldn't disconnect " << dis_pool << " from " << this << END_;
        return EXCEPTION_THROW;
    }
}

ret_t pool::check_for_connection(pool *node)
{
    if 
    (
        (this->parent == node || node->parent == this)
        || (forest::find_tree_link(this, node) != LINK_NOT_FOUND)
    )
    return ALREADY_CONNECT;

    return 0;
}

ret_t pool::reconnect_to_side_link(pool *side_pool)
{
    counter_t link_ind = forest::find_tree_link(side_pool);
    if (link_ind == LINK_NOT_FOUND)
    {
        _LOG[ERROR] << "Expected link not found" << END_;
        return LINK_NOT_FOUND;
    }

    std::pair<pool *, pool *> link = forest::linked_trees[link_ind];

    forest::linked_trees.erase(forest::linked_trees.begin() + link_ind);

    link.first->side_link--;
    link.second->side_link--;

    if (link.first == side_pool)
        connect_linked_trees(link.first, link.second);
    else
        connect_linked_trees(link.second, link.first);

    return 0;
}

void pool::connect_linked_trees(pool *conn, pool *to)
{
    conn->parent = to;
    
    to->children.push_back(conn);
}

bool pool::delete_child(pool *ch)
{
    for (int i = 0; i < children.size(); i++)
        if (children[i] == ch)
        {
            children.erase(children.begin() + i);
            return false;
        }

    return true;
}