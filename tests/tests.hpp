#ifndef TESTS_H
#define TESTS_H

#include <gtest/gtest.h>
#include "pool.hpp"
#include "logging.hpp"

using namespace pooling;                                                     

TEST(creating_pools, create_pool_test)
{
    pool *p = pool::create_new_pool();

    EXPECT_NE(p, nullptr);

    pool::delete_all_pools();
}

TEST(creating_pools, add_water_test)
{
    pool *p = pool::create_new_pool();

    p->add_water(10);

    EXPECT_EQ(p->show_water_volume(), 10);

    pool::delete_all_pools();
}

TEST(connecting_pools, connect_single_pools)
{
    pool *p = pool::create_new_pool();
    p->connect_pool(pool::create_new_pool());

    p->add_water(30);

    EXPECT_EQ(p->show_water_volume(), 15);

    pool::delete_all_pools();
}

TEST(connecting_pools, connect_connected_pools)
{
    pool *p1 = pool::create_new_pool();
    pool *p2 = pool::create_new_pool();
    pool *p3 = pool::create_new_pool();
    pool *p4 = pool::create_new_pool();

    EXPECT_EQ(p1->connect_pool(p2), 0);
    EXPECT_EQ(p1->connect_pool(p3), 0);
    EXPECT_EQ(p2->connect_pool(p4), 0);

    pool *p5 = pool::create_new_pool();
    pool *p6 = pool::create_new_pool();
    pool *p7 = pool::create_new_pool();

    EXPECT_EQ(p5->connect_pool(p6), 0);
    EXPECT_EQ(p5->connect_pool(p7), 0);

    EXPECT_EQ(p5->connect_pool(p3), 0);

    pool::delete_all_pools();
}

TEST(connecting_pools, connect_again)
{
    pool *p1 = pool::create_new_pool();
    pool *p2 = pool::create_new_pool();

    EXPECT_EQ(p1->connect_pool(p2), 0);
    EXPECT_EQ(p1->connect_pool(p2), ALREADY_CONNECT);

    pool::delete_all_pools();
}

TEST(disconnecting_pools, simple_disconnect)
{
    pool *p1 = pool::create_new_pool();
    pool *p2 = pool::create_new_pool();

    p1->connect_pool(p2);
    p1->add_water(30);
    p1->disconnect_pool(p2);

    p1->add_water(5);

    EXPECT_EQ(p1->show_water_volume(), 20);

    pool::delete_all_pools();
}

TEST(disconnecting_pools, reverse_simple_disconnect)
{
    pool *p1 = pool::create_new_pool();
    pool *p2 = pool::create_new_pool();

    p1->connect_pool(p2);
    p1->add_water(30);
    p2->disconnect_pool(p1);

    p1->add_water(5);

    EXPECT_EQ(p1->show_water_volume(), 20);

    pool::delete_all_pools();
}

TEST(disconnecting_pools, miss_disconnect)
{
    pool *p1 = pool::create_new_pool();
    pool *p2 = pool::create_new_pool();

    EXPECT_EQ(p1->disconnect_pool(p2), NO_CONNECTION);

    pool::delete_all_pools();
}

TEST(disconnecting_pools, link_reconnect)
{
    pool *p1 = pool::create_new_pool();
    pool *p2 = pool::create_new_pool();
    pool *p3 = pool::create_new_pool();

    p1->connect_pool(p2);
    p1->connect_pool(p3);

    p2->connect_pool(p3);

    p1->add_water(30);

    EXPECT_EQ(p1->disconnect_pool(p3), 0);

    p1->add_water(30);

    EXPECT_EQ(p2->show_water_volume(), 20);

    EXPECT_EQ(p2->disconnect_pool(p3), 0);

    p3->add_water(10);

    EXPECT_EQ(p3->show_water_volume(), 30);

    pool::delete_all_pools();
}

TEST(disconnecting_pools, link_disconnect)
{
    pool *p1 = pool::create_new_pool();
    pool *p2 = pool::create_new_pool();
    pool *p3 = pool::create_new_pool();

    p1->connect_pool(p2);
    p1->connect_pool(p3);
    
    p2->connect_pool(p3);

    EXPECT_EQ(p2->disconnect_pool(p3), 0);

    pool::delete_all_pools();
}


#endif