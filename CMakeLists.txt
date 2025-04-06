cmake_minimum_required(VERSION 3.2)

project(pool_task)

add_subdirectory(src)
add_subdirectory(logging_lib)

add_definitions(
    -D _DEBUG -g -ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat
-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy
-Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op
-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow
-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn
-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef
-Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers
-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector
-fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla
)

option(COMPILE_TESTS "tests compiling" ON)

if (COMPILE_TESTS)
    find_package(GTest REQUIRED)

    include_directories(${GTEST_INCLUDE_DIRS})

    add_subdirectory(tests)
endif ()

add_executable(big_test main.cpp)

target_include_directories(
    big_test
    PRIVATE
    ./src/pool_class
    ./src/big_data_testing
)

target_link_libraries(big_test pool_class big_testing)