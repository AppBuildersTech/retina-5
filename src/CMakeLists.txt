cmake_minimum_required (VERSION 2.6) 

project (Project)
include_directories (Project)

add_subdirectory (RetinaCore)
 
list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

list( APPEND CUDA_NVCC_FLAGS "-std=c++11 ${CUDA_NVCC_FLAGS}")

FIND_PACKAGE(CUDA REQUIRED)
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_20, code=sm_20)

set (HEADERS 
	algorithms/Retina.h
	optimizations/Grid.h
	algorithms/Physics.h
	optimizations/GridOptimization.h
        Data.h
        ExecuteRetina.h
        Tools.h
)
 
set (SOURCES 
	algorithms/Retina.cpp
	algorithms/Physics.cpp
        optimizations/Grid.cpp
	IndependentEntrypoint.cpp
        ExecuteRetina.cpp
        Tools.cpp
)
add_executable (retina ${HEADERS} ${SOURCES})
target_link_libraries (retina gpuacceleration)
