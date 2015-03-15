#include "physics.h"

TrackPure operator*(const TrackPure& one, const double alpha)
{
  return TrackPure(
      one.getXOnZ0() * alpha,
      one.getYOnZ0() * alpha,
      one.getDxOverDz() * alpha,
      one.getDyOverDz() * alpha
  );
}

TrackPure operator+(const TrackPure& one, const TrackPure& other) 
{
  return TrackPure(
    one.getXOnZ0() + other.getXOnZ0(),
    one.getYOnZ0() + other.getYOnZ0(),
    one.getDxOverDz() + other.getDxOverDz(),
    one.getDyOverDz() + other.getDyOverDz()
  );
}

inline double square(double x)
{
  return x * x;
}

double getDistanceFromTrackToHit(const TrackPure& track, const Hit& hit)
{
  return square(hit.x - track.getXOnZ0() - track.getDxOverDz() * hit.z) + square(hit.y - track.getYOnZ0() - track.getDyOverDz() * hit.z);
}
