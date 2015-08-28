#pragma once

#include "../Data.h"

#include <vector>
#include <cstdint>
#include <stdexcept>

const int GRID_SIZE = 8;
const int GRID_SIZE_X_ON_Z0 = GRID_SIZE;
const int GRID_SIZE_Y_ON_Z0 = GRID_SIZE;
const int GRID_SIZE_DX_OVER_DZ = GRID_SIZE;
const int GRID_SIZE_DY_OVER_DZ = GRID_SIZE;

const double RETINA_SHARPNESS_COEFFICIENT = 1e-3;
const double TRACK_THRESHOLD = 0.9999;
const double PARAM_TOLERANCE = 0.6;
const double SCATTER_TOLERANCE = 0.000016;

        
struct TrackPure { 
public:
  double xOnZ0;
  double yOnZ0;
  double dxOverDz;
  double dyOverDz;
  TrackPure(double x0, double y0, double tx, double ty) noexcept : 
    xOnZ0(x0), yOnZ0(y0), dxOverDz(tx), dyOverDz(ty)  {}
  TrackPure(const Hit&, const Hit&);
  TrackPure() = default;
};

TrackPure operator*(const TrackPure& one, const double alpha);

TrackPure operator+(const TrackPure& one, const TrackPure& other);

double getDistance(const TrackPure& track, const Hit& hit) noexcept;
double getDistanceDx(const TrackPure& track, const Hit& hit) noexcept;
double getDistanceDy(const TrackPure& track, const Hit& hit) noexcept;

bool isFit(const TrackPure& track, const Hit& hit, double zStart) noexcept;