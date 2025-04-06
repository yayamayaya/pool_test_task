#ifndef POOL_CLASS_H
#define POOL_CLASS_H

#include <vector>
#include <unordered_map>
#include "logging.hpp"

namespace pooling
{
    enum Messages
    {
        EXCEPTION_THROW = 81,
        CHILD_NOT_FOUND = 82,
        LINK_NOT_FOUND  = -2,
        ALREADY_CONNECT = 11
    };
    
    using volume_t   = double;
    
    using pool_ind_t = int;
    
    using counter_t  = int;

    class pool
    {
        class forest
        {
        public:
            friend class pool;
            
            struct tree_data
            {
                tree_data(): volume(0), tree_size(1) {}
                
                tree_data(volume_t vol, counter_t size): volume(vol), tree_size(size) {}
                
                volume_t   volume;
                
                counter_t  tree_size;
            };
                    
            static pool *convert_tree_to_a_subtree(pool *tree_node);

            static std::unordered_map<pool *, tree_data> forest_map;

            static void add_new_tree(pool *new_tree);
            
            static void rm_tree(pool *rmv_tree);
            
            static tree_data &return_tree_info(pool *root);

            static std::vector<std::pair<pool *, pool *>> linked_trees;

            static void add_tree_link(pool *f_tree, pool *s_tree);

            static counter_t find_tree_link(pool *tree);

            static counter_t find_tree_link(pool *f, pool *s);
        };
        
        pool *parent;
        
        std::vector<pool *> children;
        
        counter_t side_link;
        
        pool(): parent(nullptr), children(), side_link(false) {}
        
        ~pool();
        
        counter_t get_tree_size();

        counter_t count_subtree_size();
        
        pool *return_root();    
        
        ret_t delete_child(pool *ch);

        static ret_t log_exception(const char *msg);
        
        static ret_t reconnect_to_side_link(pool *side_pool);

        static void connect_linked_trees(pool *first, pool *second);
        
        ret_t check_for_connection(pool *s);

    public:
    
        static pool *create_new_pool();
        
        static void delete_all_pools();

        void add_water(volume_t vol);
        
        volume_t show_water_volume();

        ret_t connect_pool(pool *conn_pool);
        
        ret_t disconnect_pool(pool *dis_pool);

        const pool& operator =(const pool &arg) =delete;

        pool(const pool &cp) =delete;
    };
};


#endif