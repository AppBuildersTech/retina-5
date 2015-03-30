#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

const int GRID_SIZE_X_ON_Z0 = 10;
const int GRID_SIZE_Y_ON_Z0 = 10;
const int GRID_SIZE_DX_OVER_DZ = 5;
const int GRID_SIZE_DY_OVER_DZ = 5;

const double RETINA_SHARPNESS_COEFFICIENT = 100;

const int MAX_TRACK_SIZE = 24;
struct ParametrsSpaceInfo
{
  double minX0, maxX0;
  double minY0, maxY0;
  double minDxOverDz, maxDxOverDz;
  double minDyOverDz, maxDyOverDz;
};

        
struct Hit {
  float const x;
  float const y;
  float const z;
  uint32_t const id;
  Hit(float x, float y, float z, uint32_t id) :
    x(x),
    y(y),
    z(z),
    id(id)
  {
  }
};

struct TrackPure { 
    //coefficients of lineEquation x = track.xOnZ0 + track.dxOverDz * z
    //                            y = track.yOnZ0 + track.dyOverDz * z;
private:
  float xOnZ0;
  float yOnZ0;
  float dxOverDz;
  float dyOverDz;
public:
  TrackPure(float x0, float y0, float tx, float ty) noexcept : xOnZ0(x0), yOnZ0(y0), dxOverDz(tx), dyOverDz(ty)  {}
  TrackPure() = default;
  TrackPure(TrackPure&&) = default;
  TrackPure& operator=(const TrackPure& other) = default;
  
  inline float getXOnZ0() const noexcept
  {
    return xOnZ0;
  }  
  inline float getYOnZ0() const noexcept 
  {
    return yOnZ0;  
  }  
  inline float getDxOverDz() const noexcept 
  {
    return dxOverDz;
  }
  inline float getDyOverDz() const noexcept
  {
    return dyOverDz;
  }  
};

TrackPure operator*(const TrackPure& one, const double alpha);

TrackPure operator+(const TrackPure& one, const TrackPure& other);

struct Track {
public:
  int hitsNum;
  int hits[MAX_TRACK_SIZE];
  inline void addHit(int hitId) noexcept
  {
    if(hitsNum < MAX_TRACK_SIZE)
      hits[hitsNum++] = hitId;
  }
  
};



double getDistanceFromTrackToHit(const TrackPure& track, const Hit& hit) noexcept;

std::vector<Hit> parseHitsFromInput(uint8_t * input, size_t size);

ParametrsSpaceInfo generateParemetrsFromData(const std::vector<Hit>& hits);
