#pragma once

#include "../Data.h"

#include <vector>
#include <cstdint>
#include <stdexcept>

const double RETINA_SHARPNESS_COEFFICIENT = 1e-3;
const double PARAM_TOLERANCE = 0.6;
const double SCATTER_TOLERANCE = 0.000016;

        
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

TrackPure operator*(const TrackPure& one, const double alpha);

TrackPure operator+(const TrackPure& one, const TrackPure& other);

struct TrackProjection
{

  TrackProjection(double dx, double x0) :
  dx(dx), x0(x0) {
  }

  double x0;
  double dx;
};

TrackProjection operator*(const TrackProjection& one, const double alpha);

TrackProjection operator+(const TrackProjection& one, const TrackProjection& other);


double getDistance(const TrackPure& track, const Hit& hit) noexcept;
double getDistanceDx(const TrackProjection& track, const Hit& hit) noexcept;
double getDistanceDy(const TrackProjection& track, const Hit& hit) noexcept;

bool isFit(const TrackPure& track, const Hit& hit, double zStart) noexcept;