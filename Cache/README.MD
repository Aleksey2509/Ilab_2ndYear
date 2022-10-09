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

## __Warning!__

double-queue cache is constructed to have capacity to be equal at least 3 
