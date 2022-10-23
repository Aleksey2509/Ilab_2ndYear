find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRECTORIES})

set(PLANE_TEST_SRC test_plane.cc)
set(PLANE_TEST test_plane)
add_executable(${PLANE_TEST} ${PLANE_TEST_SRC})

target_link_libraries(${PLANE_TEST} geometry3D GTest::Main)

add_custom_target(plane_test
		  COMMENT "Running tests for plane"
		  COMMAND ./${PLANE_TEST})

add_dependencies(${PLANE_TEST} geometry3D)