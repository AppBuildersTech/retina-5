set (PROJECT retina) 

cmake_minimum_required (VERSION 2.6) 

project (${PROJECT}) 
 
list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

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
	algorithms/GpuRetina.cuh
)
 
set (SOURCES 
	algorithms/Retina.cpp
	algorithms/Physics.cpp
        optimizations/Grid.cpp
	IndependentEntrypoint.cpp
        ExecuteRetina.cpp
        Tools.cpp
)



cuda_compile(GpuRetina_O algorithms/GpuRetina.cu)

add_executable (${PROJECT} ${HEADERS} ${SOURCES} ${GpiRetina_O})
