#Required for CUDA-Check
cmake_minimum_required(VERSION 2.8.10)

project(gpulib)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/cuda" ${CMAKE_MODULE_PATH})
# ============================================
# === Target
# ============================================
file(GLOB_RECURSE gpuacc_SRCS "*.cu")

include_directories(.)

# ---------------------------------------
# Find Cuda
find_package(CUDA REQUIRED)

set(CUDA_ATTACH_VS_BUILD_RULE_TO_CUDA_FILE ON)

set(BUILD_SHARED_LIBS OFF)

set(CUDA_SEPARABLE_COMPILATION ON)
list(APPEND CUDA_NVCC_FLAGS -arch=sm_20 -std=c++11)

set(LIB_NAME "gpuacceleration")
cuda_add_library(${LIB_NAME}
  ${gpuacc_SRCS} 
)

set(PUBLIC_HEADERS "GpuRetina.cuh;Definition.cuh")

INSTALL(FILES ${PUBLIC_HEADERS} DESTINATION include)
INSTALL(FILES "${CMAKE_BINARY_DIR}/src/lib${LIB_NAME}.a" DESTINATION lib)