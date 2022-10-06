#include "cache2Q.hh"

int getPage(int key) { return key; }

int main()
{
    int cacheSize, requestNum;
    int hits = 0;

    std::cin >> cacheSize >> requestNum;

    cache::Cache2Q<int> Q2(cacheSize);

    for (int i = 0; i < requestNum; ++i)
    {
        int key;
        std::cin >> key;
        if (Q2.fetch(key, getPage))
            hits += 1;
    }

    std::cout << "Hits = " << hits << std::endl;
}