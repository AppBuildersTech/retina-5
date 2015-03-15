set (PROJECT retina) 

cmake_minimum_required (VERSION 2.6) 

project (${PROJECT}) 
 
list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

FIND_PACKAGE(CUDA REQUIRED)
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_20, code=sm_20)

set (HEADERS 
	cpuRetina.h
	FileStdLogger.h
	grid.h
	ipch
	physics.h
        ExecuteRetina.h
        Tools.h
)
 
set (SOURCES 
	cpuRetina.cpp
	IndependentEntrypoint.cpp
	Logger.cpp
	physics.cpp
        ExecuteRetina.cpp
        grid.cpp
        Tools.cpp
)

add_executable (${PROJECT} ${HEADERS} ${SOURCES})
