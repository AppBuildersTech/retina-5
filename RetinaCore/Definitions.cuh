/* 
 * File:   Definitions.cuh
 * Author: towelenee
 *
 * Created on August 30, 2015, 9:21 PM
 */

#pragma once
#include <cstdint>
const int MAX_TRACK_SIZE = 24;

struct Track 
{
  int hitsNum;
  int hits[MAX_TRACK_SIZE];
  inline void addHit(int hitId)
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

struct TrackPure { 
public:
  double x0;
  double y0;
  double dx;
  double dy;
  TrackPure(double x0, double y0, double tx, double ty) noexcept : 
    x0(x0), y0(y0), dx(tx), dy(ty)  {}
  TrackPure(const Hit&, const Hit&);
  TrackPure() = default;
};

struct TrackProjection
{

  TrackProjection(double dx, double x0) :
  dx(dx), x0(x0) {
  }
  
  TrackProjection() = default;

  double x0;
  double dx;
};
