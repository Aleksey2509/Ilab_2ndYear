find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRECTORIES})

set(CACHE_TEST_SRC cacheTest.cc)
set(CACHE_TEST_EXECUTABLE cacheTests)
add_executable(${CACHE_TEST_EXECUTABLE} ${CACHE_TEST_SRC})

target_link_libraries(${CACHE_TEST_EXECUTABLE} Cache2Q GTest::Main)

add_custom_target(test_caches
		  COMMENT "Running tests"
		  COMMAND ./${CACHE_TEST_EXECUTABLE})

add_dependencies(${CACHE_TEST_EXECUTABLE} Cache2Q)
