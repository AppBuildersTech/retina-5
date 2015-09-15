/* 
 * File:   CpuRetina.h
 * Author: towelenee
 *
 * Created on September 3, 2015, 4:25 PM
 */

#pragma once
#include <cmath>
#include "Definitions.cuh"
#include <iostream>

void getRetina2dCpu(
  const TrackProjection* tracks, 
  int tracksNum, 
  const double* hitsX,
  const double* hitsZ,
  int hitsNum, 
  double sharpness,
  double *values
)
{
  for (int trackId = 0; trackId < tracksNum; trackId++)
  {
    double sum = 0;
    for (int hitId = 0; hitId < hitsNum; hitId++)
    {
      double dx = hitsX[hitId] - tracks[trackId].x0 - hitsZ[hitId] * tracks[trackId].dx;
      sum += exp (-(dx * dx) * sharpness);
    }
    values[trackId] = sum;
  }
  
}

void getRetina3dCpu(
  const TrackPure* tracks, 
  int tracksNum, 
  const Hit* hits,
  int hitsNum, 
  double sharpness,
  double *values
)
{
  for (int trackId = 0; trackId < tracksNum; trackId++)
  {
    double sum = 0;
    for (int hitId = 0; hitId < hitsNum; hitId++)
    {
      double dx = hits[hitId].x - tracks[trackId].x0 - hits[hitId].z * tracks[trackId].dx;
      double dy = hits[hitId].y - tracks[trackId].y0 - hits[hitId].z * tracks[trackId].dy;
      sum += exp (-(dx * dx + dy * dy) * sharpness);
    }
    values[trackId] = sum;
  }
}
