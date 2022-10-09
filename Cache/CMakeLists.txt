#Minimum cmake version
cmake_minimum_required(VERSION 3.11)
#Project info
project(cache VERSION 0.1)
#Choose language
enable_language(CXX)
#Add options
add_compile_options(-Wall -g -ggdb)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CACHE_INCLUDES include)

add_library(Cache INTERFACE)

target_include_directories(Cache INTERFACE ${CACHE_INCLUDES})

set(SRC_DIR source)

set(CACHE2Q_SRC ${SRC_DIR}/cache2Q_main.cc)
set(CACHE2Q cache2Q)
add_executable(${CACHE2Q} ${CACHE2Q_SRC})

set(IDEAL_CACHE_SRC ${SRC_DIR}/cache_ideal_main.cc)
set(IDEAL_CACHE ideal_cache)
add_executable(${IDEAL_CACHE} ${IDEAL_CACHE_SRC})

target_link_libraries(${CACHE2Q} Cache)
target_link_libraries(${IDEAL_CACHE} Cache)

enable_testing()

add_subdirectory(tests)
