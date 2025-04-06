
add_library(big_testing big_data.cpp big_data.hpp timer.hpp)

target_link_libraries(big_testing logging pool_class)

target_include_directories(
    big_testing
    PRIVATE
    ./../pool_class
)