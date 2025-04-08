#ifndef POOL_CLASS_H
#define POOL_CLASS_H

#include <vector>
#include "logging.hpp"

struct tree_data;

namespace pooling
{
    constexpr ret_t EXCEPTION_THROW = 11;
    
    constexpr ret_t ALREADY_CONNECT = 12;

    constexpr ret_t NO_CONNECTION   = 13;

    constexpr ret_t WRONG_CHILD_IND = 14;

    constexpr ret_t LINK_NOT_FOUND  = -1;

    constexpr ret_t NO_WATER_FOUND  = -2;
    
    using volume_t   = double;
    
    using pool_ind_t = int;
    
    using counter_t  = int;

    class pool
    {        
        pool *parent;
        
        std::vector<pool *> children;
        
        counter_t side_link;
        
        pool(): parent(nullptr), children(), side_link(false) {}
        
        ~pool();
                
        counter_t count_subtree_size();

        tree_data &return_tree_info();

        pool *return_root();    

        pool *convert_tree_to_subtree();
        
        bool delete_child(pool *ch);
                
        static ret_t reconnect_to_side_link(pool *side_pool);
        
        static void connect_linked_trees(pool *first, pool *second);
        
        ret_t check_for_connection(pool *s);

    public:
        counter_t children_number();
    
        bool check_for_side_links();

        static pool *create_new_pool();
        
        static void delete_all_pools();

        void add_water(volume_t vol);
        
        volume_t show_water_volume();

        ret_t connect_pool(pool *conn_pool);
        
        ret_t disconnect_pool(pool *dis_pool);

        ret_t disconnect_pool(pool_ind_t child_ind);

        const pool& operator =(const pool &arg) =delete;

        pool(const pool &cp) =delete;
    };
};


#endif