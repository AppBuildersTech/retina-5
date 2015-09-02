/* 
 * File:   Data.h
 * Author: towelenee
 *
 * Created on August 28, 2015, 9:33 PM
 */
#pragma once
#include <vector>
#include "RetinaCore/Definitions.cuh"

struct EventInfo
{
  std::vector<Hit> hits;
  int32_t sensorNum;
  double minX, minY, minZ, maxX, maxY, maxZ;
};

