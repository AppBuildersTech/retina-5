/* 
 * File:   Data.h
 * Author: towelenee
 *
 * Created on August 28, 2015, 9:33 PM
 */
#pragma once
#include <cstdint>
#include <vector>

const int MAX_TRACK_SIZE = 24;

struct Track 
{
  int hitsNum;
  int hits[MAX_TRACK_SIZE];
  inline void addHit(int hitId) noexcept
  {
    if(hitsNum < MAX_TRACK_SIZE)
      hits[hitsNum++] = hitId;
  }
  Track() : hitsNum(0) { }
  
};
        
struct Hit {
  float x;
  float y;
  float z;
  uint32_t id;
  uint32_t sensorId;
  Hit(float x, float y, float z, uint32_t id, uint32_t sensorId) :
    x(x),
    y(y),
    z(z),
    id(id),
    sensorId(sensorId)
  {
  }
  Hit() = default;
  Hit(const Hit& other) : Hit(other.x, other.y, other.z, other.id, other.sensorId)
  {
      
  }
};


struct EventInfo
{
  std::vector<Hit> hits;
  int32_t sensorNum;
  double minX, minY, minZ, maxX, maxY, maxZ;
};

