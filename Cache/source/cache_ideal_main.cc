#include "ideal_cache.hh"
#include <iostream>

int getPage(int key) { return key; }

int main()
{
    int cacheSize, requestNum;
    int hits = 0;

    std::cin >> cacheSize >> requestNum;

    std::vector<int> requests(requestNum);

    for (int i = 0; i < requestNum; ++i)
        std::cin >> requests[i];


    cache::idealCache<int> ideal(cacheSize, requests.begin(), requests.end());

    for (auto& requestIt: requests)
    {
        if (ideal.fetch(requestIt, getPage))
            hits++;
    }


    std::cout << "Hits = " << hits << std::endl;
}