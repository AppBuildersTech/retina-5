#include "Physics.h"


#include <iostream>
#include <limits>
#include <cmath>

TrackPure operator*(const TrackPure& one, const double alpha)
{
  return TrackPure(
      one.x0 * alpha,
      one.y0 * alpha,
      one.dx * alpha,
      one.dy * alpha
  );
}

TrackPure operator+(const TrackPure& one, const TrackPure& other) 
{
  return TrackPure(
    one.x0 + other.x0,
    one.y0 + other.y0,
    one.dx + other.dx,
    one.dy + other.dy
  );
}
TrackPure::TrackPure(const Hit& a, const Hit& b)
{
  dx = (b.x - a.x) / (b.z - a.z);
  dy = (b.y - a.y) / (b.z - a.z);
  x0 = a.x - a.z * dx;
  y0 = a.y - a.z * dy;
}

TrackProjection operator*(const TrackProjection& one, const double alpha)
{
  return TrackProjection(
      one.x0 * alpha,
      one.dx * alpha
  );
}

TrackProjection operator+(const TrackProjection& one, const TrackProjection& other) 
{
  return TrackProjection(
    one.x0 + other.x0,
    one.dx + other.dx
  );
}


inline double square(double x)
{
  return x * x;
}

double getDistance(const TrackPure& track, const Hit& hit) noexcept
{
  return square(hit.x - track.x0 - track.dx * hit.z) +
         square(hit.y - track.y0 - track.dy * hit.z);
}

double getDistanceDx(const TrackProjection& track, const Hit& hit) noexcept
{
  return square(hit.x - track.x0 - track.dx * hit.z);
}

double getDistanceDy(const TrackProjection& track, const Hit& hit) noexcept
{
  return square(hit.y - track.x0 - track.dx * hit.z);
}

bool isFit(const TrackPure& track, const Hit& hit, double zStart) noexcept
{
  double dx = fabs(hit.x - track.x0 - track.dx * hit.z);
  double dy = fabs(hit.y - track.y0 - track.dy * hit.z);
  double scatterNum = dx * dx + dy * dy;
  double scatterNorm = 1 / (hit.z - zStart);
  double scatter = scatterNum * scatterNorm * scatterNorm;
  return dx < PARAM_TOLERANCE && dy < PARAM_TOLERANCE && scatter < SCATTER_TOLERANCE;
}