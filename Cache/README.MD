# __Cache Task__

## __Task Formulation__

Implement ideal cache and double-queue cache

## __Build__

Run in Cache folder:
```
mkdir build
cd build
cmake ..
make
```

To run tests for double-queued cache:
```
make test_2Q
```

To run tests for ideal cache:
```
make test_ideal
```

## __Usage__

You get 2 executables: cache2Q and ideal_cache, which take cache capacity and page requests as input and give number of hits as output

## __Warnings__

* Double-queue cache is constructed to have capacity to be equal at least 3

* One may encounter the fact that at small capacities double-queue cache is severely losing to LRU cache in amount of hits. This is due to the fact when the total capacity of double-queue cache is small it's main purpose is lost: pages which are accessed several times in programm, but not in row just cannot get into the Am buffer (whole purpose of which is to contain those types of pages) because after being cached they get pushed out of Ain and then from Aout even before they are accessed the next time. As a concequence, Am buffer just stays empty, and effective capacity becomes lower 