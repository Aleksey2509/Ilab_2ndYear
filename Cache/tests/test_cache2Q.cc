#include <gtest/gtest.h>

#include "cache2Q.hh"

int getPage (int pageKey)
{
    return pageKey;
}

TEST(LRUCacheTest, Test)
{
    cache::CacheLRU<int> cache{4};
    std::vector input = {1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};
    int hits = 0;
    int inputSize = input.size();
    for (int i = 0; i < inputSize; i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 6);
}


TEST(Cache2QTest, Test1)
{
    cache::Cache2Q<int> cache{15};
    std::vector input = {1, 2, 3, 4, 5, 6, 7, 1, 2, 6};
    int hits = 0;
    int inputSize = input.size();
    for (int i = 0; i < inputSize; i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 3);
}

TEST(Cache2QTest, Test2)
{
    cache::Cache2Q<int> cache{4};
    std::vector input = {1, 2, 3, 4, 2, 3, 4, 4};
    int hits = 0;
    int inputSize = input.size();
    for (int i = 0; i < inputSize; i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 4);
}

TEST(IdealCacheTest, Test3)
{
    cache::Cache2Q<int> cache{3};
    std::vector input = {1, 5, 1, 4, 6, 7, 5, 3, 0, 4, 1, 1, 3, 4, 5, 7, 9, 2, 6, 1};
    int inputSize = input.size();
    int hits = 0;
    for (int i = 0; i < inputSize; i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 4);
}


TEST(IdealCacheTest, Test4)
{
    cache::Cache2Q<int> cache{4};
    std::vector input = {2, 6, 7, 3, 6, 10, 2, 4, 6, 3, 4, 10, 5, 4, 7, 9, 10, 2, 6, 5, 1, 7, 11, 0, 6, 4, 0, 2, 1, 3};
    int hits = 0;
    int inputSize = input.size();
    for (int i = 0; i < inputSize; i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 6);
}

TEST(Cache2QTest, Test5)
{
    cache::Cache2Q<int> cache{1};
    std::vector input = {1, 2, 3, 4, 5, 5, 5, 1, 2, 3};
    int hits = 0;
    int inputSize = input.size();
    for (int i = 0; i < inputSize; i++)
    {
        hits += cache.fetch(input[i], getPage);
    }

    EXPECT_EQ(hits, 2);
}

