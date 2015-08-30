#include "GpuRetina.cuh"
#include <cstdio>

void getRetinaDx(
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