cmake_minimum_required(VERSION 2.6)

find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories (${TEST_SOURCE_DIR}/src
                     ${Boost_INCLUDE_DIRS}
                     )
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -lboost_unit_test_framework")

add_definitions(-std=c++11)
add_definitions (-DBOOST_TEST_DYN_LINK)
add_executable (GpuTest GpuTest.cpp)
target_link_libraries (GpuTest
                       gpuacceleration
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )
add_executable (ToolsTest ToolsTest.cpp ../src/Tools.cpp)
target_link_libraries (ToolsTest
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )
