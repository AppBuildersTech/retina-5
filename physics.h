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

const int MAX_TRACK_SIZE = 24;

struct Hit {
  float x;
  float y;
  float z;
  uint32_t id;
};

struct Track {
  float x0;
  float y0;
  float tx;
  float ty;

  int hitsNum;
  int hits[MAX_TRACK_SIZE];
};

struct TrackPure { 
    //coefficients of lineEquation x = track.xOnZ0 + track.dxOverDz * z
    //                            y = track.yOnZ0 + track.dyOverDz * z;
public:
  float const xOnZ0;
  float const yOnZ0;
  float const dxOverDz;
  float const dyOverDz;  
  TrackPure(float x0, float y0, float tx, float ty) : xOnZ0(x0), yOnZ0(y0), dxOverDz(tx), dyOverDz(ty) {}
  TrackPure() = default;
  TrackPure(TrackPure&&) = default;
  
  TrackPure& operator=(const TrackPure& other) 
  {    
      return (*this) = other;
  }
  
};

TrackPure operator*(const TrackPure& one, const double alpha);

TrackPure operator+(const TrackPure& one, const TrackPure& other);


double getDistanceFromTrackToHit(const TrackPure& track, const Hit& hit);

std::vector<Hit> parseHitsFromInput(uint8_t * input, size_t size);
