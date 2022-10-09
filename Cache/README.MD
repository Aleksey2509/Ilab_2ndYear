# Cache Task

## Task Formulation

Implement ideal cache and double-queue cache

## __Warning!__

double-queue cache is constructed to have capacity to be equal at least 3 



# Build:

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

You get 2 executables: cache2Q and ideal_cache, which take cache size and page requests as input and give number of hits as output