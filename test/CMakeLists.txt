find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
#include_directories (${TEST_SOURCE_DIR}/src
#                     ${Boost_INCLUDE_DIRS}
#                     )
add_definitions(-std=c++11)
add_definitions (-DBOOST_TEST_DYN_LINK)
add_executable (GpuTest GpuTest.cpp)
target_link_libraries (GpuTest
                       gpuacceleration
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )
#add_executable (GridTest GridTest.cpp)
#target_link_libraries (GridTest
                       #${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      # )
