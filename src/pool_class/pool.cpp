#include <cassert>
#include <exception>
#include "forest_funcs.hpp"
#include "pool.hpp"

using namespace pooling;

pool *pool::create_new_pool()
{
    pool *new_pool = new pool;

    forest::add_new_tree(new_pool);

    return new_pool;
}

void pool::delete_all_pools()
{
    for (auto it = forest::forest_map.begin(); it != forest::forest_map.end(); it++)
        delete it->first;

    forest::forest_map.clear();
    forest::linked_trees.clear();
}

pool::~pool() 
{
    for (int i = 0; i < children.size(); i++)
        delete children[i];
}

void pool::add_water(volume_t vol)
{
    tree_data &tree_info = return_tree_info();

    volume_t new_vol = vol / tree_info.tree_size;
    if (tree_info.volume + new_vol < 0)
    {
        tree_info.volume = 0;
        return;
    }

    tree_info.volume += new_vol;
}

volume_t pool::show_water_volume()
{
    volume_t vol = return_tree_info().volume;
    if (vol < 0.0)
        throw std::range_error("water volume become negative");

    return vol;
}

tree_data &pool::return_tree_info()
{
    if (parent)
        return parent->return_tree_info();

    auto it = forest::forest_map.find(this);
    if (it != forest::forest_map.end())
        return forest::forest_map[this];

    throw std::logic_error("couldn't find tree info for this pool");
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

    tree_data conn_info   = conn_pool->return_tree_info();
    tree_data &data       = this     ->return_tree_info();

    counter_t this_size = data.tree_size;
    data.tree_size += conn_info.tree_size;

    data.volume = (data.volume * this_size + conn_info.volume * conn_info.tree_size) / data.tree_size;
    
    pool *transformed_pool = conn_pool->convert_tree_to_subtree();
    transformed_pool->parent = this;
    children.push_back(transformed_pool);
    
    return 0;
}

pool *pool::convert_tree_to_subtree()
{
    pool *node_parent = nullptr;
    pool *prev_node   = nullptr;
    pool *curr_node   = this;

    while (true)
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

    forest::rm_tree(curr_node);

    return this;
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

    tree_data &data = return_tree_info();
        
    counter_t subtree_size = dis_pool->count_subtree_size();
    data.tree_size -= subtree_size;

    forest::forest_map[dis_pool] = {data.volume, subtree_size};

    dis_pool->parent = nullptr;

    return 0;
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

ret_t pool::disconnect_pool(pool_ind_t child_ind)
{
    if (child_ind > children.size())
        return WRONG_CHILD_IND;

    return disconnect_pool(children[child_ind]);
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

counter_t pool::count_subtree_size()
{
    counter_t cnt = 1;

    for (int i = 0; i < children.size(); i++)
        cnt += children[i]->count_subtree_size();
    
    return cnt;
}

void pool::connect_linked_trees(pool *conn, pool *to)
{
    conn->parent = to;
    
    to->children.push_back(conn);
}

counter_t pool::children_number()
{
    return children.size();
}

bool pool::check_for_side_links()
{
    return side_link;
}