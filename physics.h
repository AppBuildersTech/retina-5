#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

const double MIN_X_ON_Z0 = -100, MAX_X_ON_Z0 = 100;
const int GRID_SIZE_X_ON_Z0 = 5;

const double MIN_Y_ON_Z0 = -100, MAX_Y_ON_Z0 = 100;
const int GRID_SIZE_Y_ON_Z0 = 5;

const double MIN_DX_OVER_DZ = -100, MAX_DX_OVER_DZ = 100;
const int GRID_SIZE_DX_OVER_DZ = 5;

const double MIN_DY_OVER_DZ = -100, MAX_DY_OVER_DZ = 100;
const int GRID_SIZE_DY_OVER_DZ = 5;

const double RETINA_SHARPNESS_COEFFICIENT = 1;

const int MAX_TRACK_SIZE = 24;

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
  TrackPure(float x0, float y0, float tx, float ty) : xOnZ0(x0), yOnZ0(y0), dxOverDz(tx), dyOverDz(ty) {}
  TrackPure() = default;
  TrackPure(TrackPure&&) = default;
  TrackPure& operator=(const TrackPure& other) = default;
  
  inline float getXOnZ0() const
  {
    return xOnZ0;
  }  
  inline float getYOnZ0() const
  {
    return yOnZ0;  
  }  
  inline float getDxOverDz() const
  {
    return dxOverDz;
  }
  inline float getDyOverDz() const
  {
    return dyOverDz;
  }  
};

TrackPure operator*(const TrackPure& one, const double alpha);

TrackPure operator+(const TrackPure& one, const TrackPure& other);

struct Track {
private:
  float xOnZ0;
  float yOnZ0;
  float dxOverDz;
  float dyOverDz;
  int hitsNum;
  int hits[MAX_TRACK_SIZE];
public:
  Track(const TrackPure& track) : 
    xOnZ0(track.getXOnZ0()), 
    yOnZ0(track.getYOnZ0()), 
    dxOverDz(track.getDxOverDz()), 
    dyOverDz(track.getDyOverDz())
  {      
  }
  Track() = default;
  Track(const Track& other) = default;
  Track& operator=(const Track& other) = default;
  Track(Track&&) = default;
  
  void addHit(int hitId)
  {
    if(hitsNum < MAX_TRACK_SIZE)
      hits[hitsNum++] = hitId;
  }
  
};



double getDistanceFromTrackToHit(const TrackPure& track, const Hit& hit);

std::vector<Hit> parseHitsFromInput(uint8_t * input, size_t size);
