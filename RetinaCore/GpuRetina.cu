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

template<class T>
T* allocAndFetch(const T* data, int size)
{
  T* answer = nullptr;
  cudaMalloc( (void**)&answer, sizeof(T) * size);
  cudaMemcpy( answer, data, sizeof(T) * size, cudaMemcpyHostToDevice);
  return answer;
}

void getRetinaDxGpu(
  const TrackProjection* tracks, 
  int tracksNum, 
  const double* hitsX,
  const double* hitsZ,
  int hitsNum, 
  double sharpness,
  double *values
) {
  TrackProjection* tracksGpu = allocAndFetch(tracks, tracksNum);
  double* hitsXGpu = allocAndFetch(hitsX, hitsNum);
  double* histZGpu = allocAndFetch(hitsZ, hitsNum);
  double* valuesGpu = nullptr;
  cudaMalloc( (void**)&valuesGpu, sizeof(double) * tracksNum);
  calculateRetina2d<128><<<tracksNum, 128>>>(
    tracksGpu, 
    tracksNum, 
    hitsXGpu, 
    histZGpu, 
    hitsNum, 
    sharpness, 
    valuesGpu
  );
  cudaMemcpy( valuesGpu, values, sizeof(double) * tracksNum, cudaMemcpyDeviceToHost );
}


void getRetinaDxCpu(
  const TrackProjection* tracks, 
  int tracksNum, 
  const Hit* hits,
  int hitsNum, 
  double sharpness,
  double *values
) {
  int trackId, hitId;
  for (trackId = 0; trackId < tracksNum; ++trackId)
  {
    if (trackId % 1000 == 0)
      printf("%d\n", trackId);
    const double trackX0 = tracks[trackId].x0;
    const double trackDx = tracks[trackId].dx;
    double sum = 0;
    for (hitId = 0; hitId < hitsNum; hitId++)
    {
      const double hitX = hits[hitId].x;
      const double hitZ = hits[hitId].z;
      const double shift = (hitX - trackX0 - hitZ * trackDx);
      sum += exp(-shift * shift * sharpness);
    }
    values[trackId] = sum;
  }
}
/*
void getRetinaDy(
  const TrackProjection* tracks, 
  int tracksNum, 
  const Hit* hits,
  int hitsNum, 
  double sharpness,
  double *values
) {
  int trackId, hitId;
  for (trackId = 0; trackId < tracksNum; ++trackId)
  {
    if (trackId % 1000 == 0)
      printf("%d\n", trackId);
    const double trackX0 = track[hitId].x;
    const double trackDx = track[hitId].dx;
    double sum = 0;
    for (hitId = 0; hitId < hitsNum; hitId++)
    {
      const double hitX = hits[hitId].y;
      const double hitZ = hits[hitId].z;
      const double shift = (hitX - trackX0 - hitZ * trackDx);
      sum += exp(-shift * shift * sharpness);
    }
    values[trackId] = sum;
  }
}
*/