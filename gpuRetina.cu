#include <cuda_runtime.h>
#include <vector>
#include "gpuRetina.cuh"
template<int BLOCK_SIZE>
__global__ void calculateResponces(
  TrackPure *grid, 
  Hit *hits, 
  int hits_size, 
  double sharpness, 
  double *responces
)
{
  __shared__ double sums[BLOCK_SIZE];
	int index = threadIdx.x;
  int tid = threadIdx.x;
  int blockId = blockIdx.x;
  double sum = 0;
  while (index < hits_size)
  {
    sum += calculateResponce(grid[blockId], hits[index], sharpness);
    index += BLOCK_SIZE;
  }
  sums[tid] = sum;
  __syncthreads();
  for (int s = BLOCK_SIZE >> 1; s != 0; s >>= 1)
  {
     if (tid < s)
       sums[tid] += sums[tid + s];
    __syncthreads();
  }
  if (tid == 0)
    responces[blockId] = sums[0];  
}

std::vector<double> gpuCalculateRetinaResponces(const std::vector<TrackPure>& grid,
  const std::vector<Hit>& hits,
  double sharpness
)
{
  std::vector<double> responces(grid.size());

  TrackPure* grid_gpu;
  cudaMalloc( (void **) &grid_gpu, grid.size() * sizeof(TrackPure));
  cudaMemcpy(grid_gpu, grid.data(), grid.size() * sizeof(TrackPure), cudaMemcpyHostToDevice);
  
  Hit* hits_gpu;
  cudaMalloc( (void **) &hits_gpu, hits.size() * sizeof(Hit));
  cudaMemcpy(hits_gpu, hits.data(), hits.size() * sizeof(Hit), cudaMemcpyHostToDevice);
  
  double *responces_gpu;
  cudaMalloc( (void **) &responces_gpu, grid.size() * sizeof(double));
  
  calculateResponces<THREADS_PER_BLOCK> <<<grid.size(), THREADS_PER_BLOCK>>> (grid_gpu, hits_gpu, hits.size(), sharpness, responces_gpu);
  
  cudaMemcpy(responces.data(), responces_gpu, grid.size() * sizeof(double), cudaMemcpyDeviceToHost);
  
  cudaFree(hits_gpu);
  cudaFree(grid_gpu);
  cudaFree(responces_gpu);
  
  return responces;
}
