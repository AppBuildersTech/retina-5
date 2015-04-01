set (PROJECT retina) 

cmake_minimum_required (VERSION 2.6) 

project (${PROJECT}) 

list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

FIND_PACKAGE(CUDA REQUIRED)

set (HEADERS 
	cpuRetina.h
	FileStdLogger.h
	grid.h
	ipch
	physics.h
        ExecuteRetina.h
        Tools.h
        #gpuRetina.cuh
) 
set (SOURCES 
	cpuRetina.cpp
	IndependentEntrypoint.cpp
	Logger.cpp
	physics.cpp
        ExecuteRetina.cpp
        grid.cpp
        Tools.cpp
        #gpuRetina.cu
)

cuda_add_executable (${PROJECT} ${HEADERS} ${SOURCES})
