cmake_minimum_required (VERSION 2.6)
project (retina)
add_definitions(-DUSE_CPU)

add_subdirectory (src) 
add_subdirectory (test)
enable_testing ()
add_test (NAME ToolsTest COMMAND ToolsTest)
if (NOT DEFINED USE_CPU)
    add_test (NAME GpuTest COMMAND GpuTest)
endif()

