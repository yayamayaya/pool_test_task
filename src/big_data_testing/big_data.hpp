#ifndef BIG_DATA_TESTING_H
#define BIG_DATA_TESTING_H

#include <cstdlib>
#include "pool.hpp"
#include "timer.hpp"

using namespace pooling;

class big_data_testing
{
    std::vector<pool *> pools;

    static inline const std::string dump_file_name = "dump.log";

    static inline unsigned int give_random(unsigned int val);

public:
    //create N pools and fill them with random amount of water
    void create_pools(size_t N);

    //add random amount of water to L random pools
    void fill_pools(size_t L);

    //show water amount in every pool
    void dump_pools();

    //connect K random pools together
    void connect_pools(size_t K);

    //break M random connections from pools
    void break_connections(size_t M);

    big_data_testing(): pools() {}

    ~big_data_testing() {pool::delete_all_pools();}
};

#endif