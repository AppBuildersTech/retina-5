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

double distributionFunction(double x, double sharpness)
{
  return exp(- x * x * sharpness);
}

double distributionFunction(double x, double y, double sharpness)
{
  return exp(- (x * x + y * y) * sharpness);
}

void getRetina2dCpu(
  const TrackProjection* tracks,
  int numberOfTracks,
  const double* hitsX,
  const double* hitsZ,
  int numberOfHits,
  double sharpness,
  double *probabilites
)
{
  for (int trackId = 0; trackId < numberOfTracks; trackId++)
  {
    double sum = 0;
    for (int hitId = 0; hitId < numberOfHits; hitId++)
    {
      sum += distributionFunction(
                hitsX[hitId] - tracks[trackId].x0 - hitsZ[hitId] * tracks[trackId].dx,
                sharpness);
    }
    probabilites[trackId] = sum;
  }

}

void getRetina3dCpu(
  const TrackPure* tracks,
  int numberOfTracks,
  const Hit* hits,
  int numberOfHits,
  double sharpness,
  double *probabilites
)
{
  for (int trackId = 0; trackId < numberOfTracks; trackId++)
  {
    double sum = 0;
    for (int hitId = 0; hitId < numberOfHits; hitId++)
    {
      sum += distributionFunction(
                  hits[hitId].x - tracks[trackId].x0 - hits[hitId].z * tracks[trackId].dx,
                  hits[hitId].y - tracks[trackId].y0 - hits[hitId].z * tracks[trackId].dy,
                  sharpness);
    }
    probabilites[trackId] = sum;
  }
}
