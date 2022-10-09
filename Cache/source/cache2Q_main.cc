#include "cache2Q.hh"
#include <iostream>

int getPage(int key) { return key; }

int main()
{
    int cacheSize, requestNum;
    int hits = 0;

    std::cin >> cacheSize >> requestNum;

    cache::Cache2Q<int> doubleQueued(cacheSize);

    for (int i = 0; i < requestNum; ++i)
    {
        int key;
        std::cin >> key;
        if (doubleQueued.fetch(key, getPage))
            hits += 1;
    }

    std::cout << "Hits = " << hits << std::endl;
}