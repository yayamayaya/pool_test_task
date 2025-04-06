#include "big_data.hpp"
#include <iomanip>

void big_data_testing::create_pools(size_t N)
{
    timer t;
    
    logger::logger_init(dump_file_name);
    
    for (size_t i = 0; i < N; i++)
    {
        pools.push_back(pool::create_new_pool());
        
        pools[i]->add_water(give_random(500));        
    }
    
    _LOG << END_;
    
    std::cout << "> " << N << " pools were created in";
}

inline unsigned int big_data_testing::give_random(unsigned int val)
{
    return static_cast<unsigned int>(std::rand()) % val;
}

void big_data_testing::fill_pools(size_t L)
{
    timer t;

    for (size_t i = 0; i < L; i++)
        pools[i]->add_water(give_random(500));

    std::cout << "> " << L << " random pools were filled with water in";
}

void big_data_testing::dump_pools()
{
    timer t;

    for (size_t i = 0; i < pools.size(); i++)
    {
        _LOG << std::fixed << std::setprecision(1) << pools[i]->show_water_volume() << "\n";
    }

    _LOG << END_;

    std::cout << "> " << "all pools volumes were printed in";
}

void big_data_testing::connect_pools(size_t K)
{
    timer t;

    for (size_t i = 0; i < K; i++)
    {
        unsigned long pool_number1 = give_random(pools.size());
        unsigned long pool_number2 = give_random(pools.size());

        pools[pool_number1]->connect_pool(pools[pool_number2]);
    }

    std::cout << "> " << K << " pools were connected together in";
}

void big_data_testing::break_connections(size_t M)
{
    timer t;

    for (size_t i = 0; i < M; i++)
    {
        unsigned long pool_number1 = give_random(pools.size());
        unsigned long pool_number2 = give_random(pools.size());

        pools[pool_number1]->disconnect_pool(pools[pool_number2]);
    }

    std::cout << "> " << M << " pools were tried to be disconnected in";
}