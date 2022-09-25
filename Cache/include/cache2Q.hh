#ifndef DOUBLEQCACHE_HH
#define DOUBLEQCACHE_HH

#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>
#include <iostream>


namespace cache
{

template <typename T, typename KeyT = int>
class CacheLRU
{
private:

    using listElem = typename std::pair<KeyT, T>;

    size_t capacity_ = 0;
    size_t size_ = 0;
    std::list<listElem> cache_;

    using ListIt = typename std::list<listElem>::iterator;
    std::unordered_map<KeyT, ListIt> cacheHash_;

    inline bool full() const { return (size_ == capacity_); }

    void pop()
    {
        cacheHash_.erase(cache_.back().first);
        cache_.pop_back();
        size_--;

        return;
    }

public:

    CacheLRU(size_t capacity) : capacity_(capacity) {}

    inline bool cached (KeyT key){ return cacheHash_.find(key) != cacheHash_.end(); }

    template <typename Func>
    bool fetch(KeyT key, Func getPage)
    {
        if (!cached(key))
        {
            addElem(key, getPage(key));

            return false;
        }

        auto requested = cacheHash_.find(key)->second;

        if (requested != cache_.begin())
            cache_.splice(cache_.begin(), cache_, requested, std::next(requested));

        return true;
    }

    void addElem(KeyT key, T elem)
    {
        if (full())
            pop();

        cache_.emplace_front(key, elem);
        cacheHash_[key] = cache_.begin();
        size_++;
    }


};

template <typename T, typename KeyT = int>
struct HashedQueue
{
    size_t capacity_ = 0;
    size_t size_ = 0;
    using listElem = typename std::pair<KeyT, T>;
    std::list<listElem> list_;

    using ListIt = typename std::list<listElem>::iterator;
    std::unordered_map<KeyT, ListIt> listHash_;

public:

    HashedQueue(size_t capacity) : capacity_(capacity) {}

    inline bool hashed (KeyT key) {return listHash_.find(key) != listHash_.end();}

    void pushFront(KeyT key, T elem)
    {
        list_.emplace_front(key, elem);
        listHash_[key] = list_.begin();
        size_++;
    }

    void erase(KeyT key)
    {
        auto toDelete = listHash_.find(key);
        list_.erase(toDelete->second);
        listHash_.erase(key);
        size_--;
    }

    void popBack()
    {
        listHash_.erase(list_.back().first);
        list_.pop_back;
        size_--;
    }

    T getElem(KeyT key) // to be used when caller already checked that elem is in queue with cached()
    {
        return listHash_.find(key)->second->second;
    }

    inline bool full() {return size_ == capacity_;}
};


template <typename T, typename KeyT = int>
class Cache2Q
{
    size_t AinCapacity_ = 0;
    size_t AoutCapacity_ = 0;

    using listElem = typename std::pair<KeyT, T>;

    HashedQueue<KeyT, T> Ain_;
    HashedQueue<KeyT, T> Aout_;

    CacheLRU<T> Am_;

    size_t AinSize_ = 0;
    size_t AoutSize_ = 0;



    static constexpr double A_IN_PART_ = 0.2;
    static constexpr double A_OUT_PART_ = 0.4;

    static constexpr size_t MIN_A_IN_SIZE = 1;
    static constexpr size_t MIN_A_OUT_SIZE = 1;
    static constexpr size_t MIN_A_M_SIZE = 1;

public:
   static constexpr size_t MIN_CACHE2Q_CAPACITY = MIN_A_OUT_SIZE + MIN_A_IN_SIZE + MIN_A_M_SIZE;

public:

    Cache2Q(size_t capacity) : AinCapacity_(std::max<size_t> (std::trunc (A_IN_PART_ * capacity), MIN_A_IN_SIZE)),
                               AoutCapacity_(std::max<size_t>(std::trunc (A_OUT_PART_ * capacity), MIN_A_M_SIZE)),
                               Ain_(AinCapacity_),
                               Aout_(AoutCapacity_),
                               Am_(std::max<size_t>(capacity - AinCapacity_ - AoutCapacity_, MIN_A_OUT_SIZE))
                               {}

    template <typename Func>
    bool fetch(KeyT key, Func getPage)
    {
        if (Am_.cached(key))
        {
            return Am_.fetch(key, getPage); // simultaniously update Am as a LRU cache
        }

        if (Ain_.hashed(key))
        {
            return true;
        }

        if (Aout_.hashed(key))
        {
            auto elem = Aout_.getElem(key);
            Am_.addElem(key, elem);
            Aout_.erase(key);
            return true;
        }

        loadNewElem<Func> (key, getPage);
        return false;
    }

    template <typename Func>
    void loadNewElem(KeyT key, Func getPage)
    {
        if (!Ain_.full())
        {
            Ain_.pushFront(key, getPage(key));
            return;
        }
        
        if (Aout_.full())
            Aout_.erase(Aout_.list_.back().first);

        auto toMove = Ain_.list_.back();
        Aout_.pushFront(toMove.first, toMove.second);

        Ain_.erase(toMove.first);
        Ain_.pushFront(key, getPage(key));
    }

};


} // namespace cache

#endif