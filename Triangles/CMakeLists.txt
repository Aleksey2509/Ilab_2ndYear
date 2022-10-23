#Minimum cmake version
cmake_minimum_required(VERSION 3.11)
#Project info
project(triangles VERSION 0.1)
#Choose language
enable_language(CXX)
#Add options
add_compile_options(-Wall -Wpedantic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GEOMETRY_INCLUDES include)

set(GEOMETRY_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(GEOMETRY_SRC ${GEOMETRY_SRC_DIR}/intersections.cc)

add_library(geometry3D)

target_include_directories(geometry3D PUBLIC ${GEOMETRY_INCLUDES})
target_sources(geometry3D PUBLIC ${GEOMETRY_SRC})

set(SRC_DIR source)

set(MAIN_SRC ${SRC_DIR}/main.cc)
set(MAIN main)
add_executable(${MAIN} ${MAIN_SRC})

target_link_libraries(${MAIN} geometry3D)

enable_testing()
add_subdirectory(tests)