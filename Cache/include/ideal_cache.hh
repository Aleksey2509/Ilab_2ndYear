#ifndef IDEAL_CACHE_HH
#define IDEAL_CACHE_HH

#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <type_traits>

namespace cache
{

template <typename T, typename KeyT = int>
class idealCache
{
private:

    using listElem = typename std::pair<KeyT, T>;

    size_t fetched_ = 0;
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
            auto firstEncounter = std::find(pageCallVector.begin(), pageCallVector.end(), it->first);
            if (firstEncounter == pageCallVector.end())
            {
                toPop = it;
                break;
            }
            else
            {
                auto dist = std::distance(pageCallVector.begin(), firstEncounter);
                if (dist > maxDist)
                {
                    maxDist = dist;
                    toPop = it;
                }
            }
        }

        return toPop;
    }

    bool full() const { return (size_ == capacity_); }

public:

    template <typename InputIt>
    idealCache(unsigned capacity, InputIt begin, InputIt end): capacity_(capacity), pageCallVector{begin, end}
    {
        static_assert(std::is_constructible_v<T, typename std::iterator_traits<InputIt>::value_type>);
    }


    template <typename Func>
    bool fetch(KeyT key, Func getPage)
    {
        pageCallVector[fetched_++] = 0;
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