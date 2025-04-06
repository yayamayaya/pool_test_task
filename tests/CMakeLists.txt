project(tests)

add_executable(tests tests.cpp)

target_link_libraries(tests pool_class logging)

target_link_libraries(tests ${GTEST_LIBRARIES} pthread)

target_include_directories(
    tests
    PRIVATE
    ./../src/pool_class
)
