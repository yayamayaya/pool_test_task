#include "pool.hpp"
#include "big_data.hpp"

using namespace pooling;

int main()
{
    timer t;

    const size_t N = 34'000'000;
    const size_t K = 12'000'000;
    const size_t L = 27'000'000;
    const size_t M = 25'000'000;

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

    std::cout << "> programm finished in";

    return 0;
}

