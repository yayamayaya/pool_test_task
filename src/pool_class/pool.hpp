#ifndef POOL_CLASS_H
#define POOL_CLASS_H

#include <vector>
#include <unordered_map>
#include "logging.hpp"

using volume_t = double;

using pool_ind_t = int;

using counter_t = int;

namespace pooling
{
    class pool;

    // extern std::vector<pool *>active_pools;

    // extern std::unordered_map<pool *, counter_t> tree_sizes;

    // extern counter_t counter;

    class pool_forest
    {
    public:
        friend class pool;
        
        struct pool_tree
        {
            pool_tree(): volume(0), tree_size(1) {}
            
            pool_tree(volume_t vol, counter_t size): volume(vol), tree_size(size) {}
            
            volume_t   volume;
            
            counter_t  tree_size;
        };
        
    private:
        
        static pool *convert_tree_to_a_subtree(pool *tree_node);

        static std::unordered_map<pool *, pool_tree> forest_map;

        static void add_new_tree(pool *new_tree);

        static void delete_tree(pool *dying_tree);

        static pool_tree &return_tree_info(pool *root);

    };

    class pool
    {
        // volume_t water_volume;

        friend pool_forest;

        pool *parent;
    
        std::vector<pool *> children;
    
        bool side_link;
    
        // static inline const pool_ind_t root = -1;

        // static inline const counter_t big_tree_if = 100; 

        counter_t get_tree_size();

        pool *return_root();    

        void delete_child(pool *ch);
        
        //pool(): parent(nullptr), children(), side_link(false) {pool_forest::add_new_tree(this);}

        pool(): parent(nullptr), children(), side_link(false) {}

    public:    
        ~pool();

        static pool *add_new_pool();
        
        static void delete_all_pools();

        void add_water(volume_t vol);
        
        volume_t show_water_volume();

        ret_t connect_pool(pool *conn_pool);
        
        // ret_t rmv_pool_connection();
        
        const pool& operator =(const pool &arg) =delete;

        pool(const pool &cp) =delete;
    };
};


#endif