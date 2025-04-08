#include "pool.hpp"
#include "big_data.hpp"

using namespace pooling;

int main()
{
    timer t;

    const size_t N = 34'000'000;
    const size_t K = 12'000'000;
    const size_t L = 27'000'000;
    const size_t M = 3'000'000;

    try
    {
        big_data_testing unit;
        
        unit.create_pools(N);
        
        _LOG << "BREAKPOINT" << END_;
        
        unit.connect_pools(K);
        unit.dump_pools();
        
        _LOG << "BREAKPOINT" << END_;
        
        unit.fill_pools(L);
        unit.dump_pools();
        
        _LOG << "BREAKPOINT" << END_;
        
        unit.break_connections(M);
        unit.fill_pools(L);
        unit.dump_pools();
        
        _LOG << "FINISHED" << END_;
    }
    catch(const std::exception& e)
    {
        std::cerr << "> Exception catch " << e.what() << std::endl;
        _LOG[ERROR] << "> Exception catch " << e.what() << END_;
    }

    std::cout << "> programm finished in";

    return 0;
}

