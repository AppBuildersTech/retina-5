#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

const int GRID_SIZE_X_ON_Z0 = 10;
const int GRID_SIZE_Y_ON_Z0 = 10;
const int GRID_SIZE_DX_OVER_DZ = 10;
const int GRID_SIZE_DY_OVER_DZ = 10;

const double RETINA_SHARPNESS_COEFFICIENT = 10;
const size_t MAX_TRACK_SIZE = 24;
        
struct Hit {
  float x;
  float y;
  float z;
  uint32_t id;
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
  TrackPure(const TrackPure&) = default;
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
