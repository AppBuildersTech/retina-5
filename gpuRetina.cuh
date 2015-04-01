/* 
 * File:   gpuRetina.cuh
 * Author: towelenee
 *
 * Created on April 1, 2015, 1:36 AM
 */

#pragma once
#include "physics.h"
const int THREADS_PER_BLOCK = 256;

std::vector<double> gpuCalculateRetinaResponces(const std::vector<TrackPure>& grid,
  const std::vector<Hit>& hits,
  double sharpness
);