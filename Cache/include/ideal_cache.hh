#ifndef IDEAL_CACHE_HH
#define IDEAL_CACHE_HH

#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace cache
{

template <typename T, typename KeyT = int>
class idealCache
{
private:

    using listElem = typename std::pair<KeyT, T>;

    size_t capacity_;
    size_t size_;
    std::list<listElem> cache_;
    std::vector<KeyT> pageCallVector;

    using ListIt = typename std::list<listElem>::iterator;
    std::unordered_map<KeyT, ListIt> cacheHash_;

    void add(KeyT key, T elem)
    {
        cache_.emplace_front(key, elem);
        cacheHash_[key] = cache_.begin();
        size_++;
    }

    void pop(KeyT key)
    {
        auto ifEqualByKey = [key](listElem iter){ return iter.first == key; };
        ListIt toPop = std::find_if(cache_.begin(), cache_.end(), ifEqualByKey);
    
        cacheHash_.erase(toPop->first);
        cache_.erase(toPop);
        size_--;

        return;
    }

    ListIt findLastToUse()
    {
        int maxDist = 0;
        ListIt toPop;

        for (auto it = cache_.begin(); it != cache_.end(); ++it)
        {
            auto last = std::find(pageCallVector.begin(), pageCallVector.end(), it->first);
            if (last == pageCallVector.end())
            {
                toPop = it;
                break;
            }
            else
            {
                auto dist = std::distance(pageCallVector.begin(), last);
                if (dist > maxDist)
                {
                    maxDist = dist;
                    toPop = it;
                }
            }
        }

        return toPop;
    }

    inline bool full() const { return (size_ == capacity_); }

public:
    idealCache(unsigned capacity): capacity_(capacity) {}


    template <typename Func>
    bool fetch(KeyT key, Func getPage)
    {
        auto ifHit = cacheHash_.find(key);

        if (ifHit == cacheHash_.end())
        {
            if (full())
                pop((findLastToUse())->first);

            add(key, getPage(key));

            return false;
        }
        else
            return true;
    }

    

};

} // namespace cache

#endif