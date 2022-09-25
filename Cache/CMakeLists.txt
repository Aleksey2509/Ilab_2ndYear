#Minimum cmake version
cmake_minimum_required(VERSION 3.11)
#Project info
project(Cache)
#Choose language
enable_language(CXX)
#Add options
add_compile_options(-Wall -g -ggdb)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CACHE_INCLUDES include)

add_library(Cache2Q INTERFACE)

target_include_directories(Cache2Q INTERFACE ${CACHE_INCLUDES})

enable_testing()

add_subdirectory(tests)
