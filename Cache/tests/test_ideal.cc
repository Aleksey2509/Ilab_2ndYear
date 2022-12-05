#include <gtest/gtest.h>

#include "ideal_cache.hh"

int getPage (int pageKey)
{
    return pageKey;
}

TEST(IdealCacheTest, Test1)
{
    std::vector input = {1, 2, 3, 4, 5, 6, 7, 1, 2, 6};
    int inputSize = input.size();

    cache::idealCache<int> cache{3, input.begin(), input.end()};
    int hits = 0;
    for (int i = 0; i < inputSize; i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 3);
}

TEST(IdealCacheTest, Test2)
{
    std::vector input = {1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};
    int inputSize = input.size();

    cache::idealCache<int> cache{4, input.begin(), input.end()};
    int hits = 0;
    for (int i = 0; i < inputSize; i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 7);
}

TEST(IdealCacheTest, Test3)
{
    std::vector input = {1, 5, 1, 4, 6, 7, 5, 3, 0, 4, 1, 1, 3, 4, 5, 7, 9, 2, 6, 1};
    int inputSize = input.size();

    cache::idealCache<int> cache{3, input.begin(), input.end()};
    int hits = 0;
    for (int i = 0; i < inputSize; i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 8);
}


TEST(IdealCacheTest, Test4)
{
    std::vector input = {2, 6, 7, 3, 6, 10, 2, 4, 6, 3, 4, 10, 5, 4, 7, 9, 10, 2, 6, 5, 1, 7, 11, 0, 6, 4, 0, 2, 1, 3};
    int inputSize = input.size();

    cache::idealCache<int> cache{4, input.begin(), input.end()};
    int hits = 0;
    for (int i = 0; i < inputSize; i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 15);
}

TEST(IdealCacheTest, Test5)
{
    std::vector input = {1, 2, 3, 4, 5, 5, 5, 1, 2, 3};
    int inputSize = input.size();

    cache::idealCache<int> cache{1, input.begin(), input.end()};
    int hits = 0;
    for (int i = 0; i < inputSize; i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 2);
}

