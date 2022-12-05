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

    size_t capacity_;
    size_t fetched_ = 0;
    size_t size_ = 0;
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

    KeyT findFutherUsed(KeyT key)
    {
        int maxDist = std::distance(pageCallVector.begin() + fetched_, std::find(pageCallVector.begin() + fetched_, pageCallVector.end(), key));
        KeyT toPop = key;

        for (auto it = cache_.begin(); it != cache_.end(); ++it)
        {
            auto firstEncounter = std::find(pageCallVector.begin() + fetched_, pageCallVector.end(), it->first);
            if (firstEncounter == pageCallVector.end())
            {
                toPop = it->first;
                return toPop;
            }
            else
            {
                auto dist = std::distance(pageCallVector.begin() + fetched_, firstEncounter);
                if (dist > maxDist)
                {
                    maxDist = dist;
                    toPop = it->first;
                }
            }
        }

        return toPop;
    }

    bool pageLastTimeFetched(KeyT key) const
    {
        return std::find(pageCallVector.begin() + fetched_, pageCallVector.end(), key) == pageCallVector.end();
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
        fetched_++;
        auto ifHit = cacheHash_.find(key);

        if (ifHit == cacheHash_.end())
        {
            if (pageLastTimeFetched(key)) // if this is the last time this page is fetched, no need to cache it
                return false;

            if (full())
            {
                auto candidate = findFutherUsed(key);
                if (candidate == key)
                    return false;

                pop(candidate);
            }

            add(key, getPage(key));

            return false;
        }

        return true;
    }

};

} // namespace cache

#endif