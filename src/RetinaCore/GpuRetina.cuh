/* 
 * File:   GpuRetina.cuh
 * Author: towelenee
 *
 * Created on August 30, 2015, 8:43 PM
 */
#pragma once
#include "Definitions.cuh"
void getRetina2dGpu(
  const TrackProjection* tracks, 
  int tracksNum, 
  const double* hitsX,
  const double* hitsZ,
  int hitsNum, 
  double sharpness,
  double *values
);

void getRetina3dGpu(
  const TrackPure* tracks, 
  int tracksNum, 
  const Hit* hits,
  int hitsNum, 
  double sharpness,
  double *values
);
