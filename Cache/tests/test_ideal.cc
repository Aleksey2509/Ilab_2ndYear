#include <gtest/gtest.h>

#include "ideal_cache.hh"

int getPage (int pageKey)
{
    return pageKey;
}

TEST(IdealCacheTest, BasicTest1)
{
    cache::idealCache<int> cache{15};
    int input[] = {1, 2, 3, 4, 5, 6, 7, 1, 2, 6};
    int hits = 0;
    for (int i = 0; i < sizeof(input) / sizeof(input[0]); i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 3);
}

TEST(IdealCacheTest, BasicTest2)
{
    cache::idealCache<int> cache{4};
    int input[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};
    int hits = 0;
    for (int i = 0; i < sizeof(input) / sizeof(input[0]); i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 7);
}
TEST(IdealCacheTest, BasicTest3)
{
    cache::idealCache<int> cache{1};
    int input[] = {1, 2, 3, 4, 5, 5, 5, 1, 2, 3};
    int hits = 0;
    for (int i = 0; i < sizeof(input) / sizeof(input[0]); i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 5);
}

