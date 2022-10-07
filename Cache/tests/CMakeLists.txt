find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRECTORIES})

set(CACHE2Q_TEST_SRC test_cache2Q.cc)
set(CACHE2Q_TEST test_cache2Q)
add_executable(${CACHE2Q_TEST} ${CACHE2Q_TEST_SRC})

set(IDEAL_CACHE_TEST_SRC test_ideal.cc)
set(IDEAL_CACHE_TEST test_ideal-cache)
add_executable(${IDEAL_CACHE_TEST} ${IDEAL_CACHE_TEST_SRC})

target_link_libraries(${CACHE2Q_TEST} Cache GTest::Main)
target_link_libraries(${IDEAL_CACHE_TEST} Cache GTest::Main)

add_custom_target(test_ideal
		  COMMENT "Running tests for ideal cache"
		  COMMAND ./${IDEAL_CACHE_TEST})

add_custom_target(test_2Q
		  COMMENT "Running tests for double-queued cache"
		  COMMAND ./${CACHE2Q_TEST})

add_dependencies(${CACHE2Q_TEST} Cache)
add_dependencies(${IDEAL_CACHE_TEST} Cache)
