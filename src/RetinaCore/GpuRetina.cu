#include "GpuRetina.cuh"
#include <cstdio>
template<int BLOCK_SIZE>
__global__ void calculateRetina2d(
  const TrackProjection* tracks, 
  int tracksNum, 
  const double* hitsX,
  const double* hitsZ,
  int hitsNum, 
  double sharpness,
  double *values
)
{
  int trackId = blockIdx.x;
  unsigned int tid = threadIdx.x;
  const double trackX0 = tracks[trackId].x0;
  const double trackDx = tracks[trackId].dx;
  double sum = 0;
  for (int hitId = tid; hitId < hitsNum; hitId += BLOCK_SIZE)
  {
    const double hitX = hitsX[hitId];
    const double hitZ = hitsZ[hitId];
    const double shift = (hitX - trackX0 - hitZ * trackDx);
    sum += exp(-shift * shift * sharpness);
  }
  __shared__ double sdata[BLOCK_SIZE];
  sdata[tid] = sum;
  for (unsigned int s = BLOCK_SIZE >> 1; s > 0; s >>= 1) 
  {
    __syncthreads();
    if (tid < s) 
    {
      sdata[tid] += sdata[tid + s];
    }
  }
  if (tid == 0)
  {
    values[trackId] = sdata[0];
  }
}

template<class T>
T* allocAndFetch(const T* data, int size)
{
  T* answer = nullptr;
  cudaMalloc( (void**)&answer, sizeof(T) * size);
  cudaMemcpy( answer, data, sizeof(T) * size, cudaMemcpyHostToDevice);
  return answer;
}

void getRetina2dGpu(
  const TrackProjection* tracks, 
  int tracksNum, 
  const double* hitsX,
  const double* hitsZ,
  int hitsNum, 
  double sharpness,
  double *values
) {
  const int BLOCK_SIZE = 1 << 8;
  TrackProjection* tracksGpu = allocAndFetch(tracks, tracksNum);
  double* hitsXGpu = allocAndFetch(hitsX, hitsNum);
  double* hitsZGpu = allocAndFetch(hitsZ, hitsNum);
  double* valuesGpu = nullptr;
  cudaMalloc( (void**)&valuesGpu, sizeof(double) * tracksNum);
  calculateRetina2d<BLOCK_SIZE><<<tracksNum, BLOCK_SIZE>>>(
    tracksGpu, 
    tracksNum, 
    hitsXGpu, 
    hitsZGpu, 
    hitsNum, 
    sharpness, 
    valuesGpu
  );
  cudaMemcpy(values, valuesGpu, sizeof(double) * tracksNum, cudaMemcpyDeviceToHost );
  cudaFree(tracksGpu);
  cudaFree(hitsXGpu);
  cudaFree(hitsZGpu);
  cudaFree(valuesGpu);
}

template<int BLOCK_SIZE>
__global__ void calculateRetina3d(
  const TrackPure* tracks, 
  int tracksNum, 
  const Hit* hits,
  int hitsNum, 
  double sharpness,
  double *values
)
{
  int trackId = blockIdx.x;
  unsigned int tid = threadIdx.x;
  const double trackX0 = tracks[trackId].x0;
  const double trackDx = tracks[trackId].dx;
  const double trackY0 = tracks[trackId].y0;
  const double trackDy = tracks[trackId].dy;
  double sum = 0;
  for (int hitId = tid; hitId < hitsNum; hitId += BLOCK_SIZE)
  {
    const float hitX = hits[hitId].x;
    const float hitY = hits[hitId].y;
    const float hitZ = hits[hitId].z;
    const double shiftX = (hitX - trackX0 - hitZ * trackDx);
    const double shiftY = (hitY - trackY0 - hitZ * trackDy);
    
    sum += exp(-(shiftX * shiftX + shiftY * shiftY) * sharpness);
  }
  __shared__ double sdata[BLOCK_SIZE];
  sdata[tid] = sum;
  __syncthreads();

  for (unsigned int s = BLOCK_SIZE >> 1; s > 0; s >>= 1) 
  {
    if (tid < s) 
    {
      sdata[tid] += sdata[tid + s];
    }
    __syncthreads();
  }
  if (tid == 0)
  {
    values[trackId] = sdata[0];
  }
}

void getRetina3dGpu(
  const TrackPure* tracks, 
  int tracksNum, 
  const Hit* hitsX,
  int hitsNum, 
  double sharpness,
  double *values
) {
  const int BLOCK_SIZE = 1 << 8;
  TrackPure* tracksGpu = allocAndFetch(tracks, tracksNum);
  Hit* hitsGpu = allocAndFetch(hitsX, hitsNum);
  double* valuesGpu = nullptr;
  cudaMalloc( (void**)&valuesGpu, sizeof(double) * tracksNum);
  calculateRetina3d<BLOCK_SIZE><<<tracksNum, BLOCK_SIZE>>>(
    tracksGpu, 
    tracksNum, 
    hitsGpu, 
    hitsNum, 
    sharpness, 
    valuesGpu
  );
  cudaMemcpy( values, valuesGpu, sizeof(double) * tracksNum, cudaMemcpyDeviceToHost );
  cudaFree(tracksGpu);
  cudaFree(hitsGpu);
  cudaFree(valuesGpu);
}

