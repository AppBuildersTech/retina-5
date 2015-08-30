/* 
 * File:   GpuRetina.cuh
 * Author: towelenee
 *
 * Created on August 30, 2015, 8:43 PM
 */
#pragma once
#include "Definitions.cuh"
void getRetinaDx(
  const TrackProjection* tracks, 
  int tracksNum, 
  const Hit* hits,
  int hitsNum, 
  double sharpness,
  double *values
);

void getRetinaDy(
  const TrackProjection* tracks, 
  int tracksNum, 
  const Hit* hits,
  int hitsNum, 
  double sharpness,
  double *values
);
