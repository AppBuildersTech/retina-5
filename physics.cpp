#include "physics.h"
#include <limits>
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

double getDistanceFromTrackToHit(const TrackPure& track, const Hit& hit) noexcept
{
  return square(hit.x - track.getXOnZ0() - track.getDxOverDz() * hit.z) +
         square(hit.y - track.getYOnZ0() - track.getDyOverDz() * hit.z);
}

ParametrsSpaceInfo generateParemetrsFromData(const std::vector<Hit>& hits)
{
  ParametrsSpaceInfo info;
  float minX, minY, minZ;
  float maxX, maxY, maxZ;
  minX = minY = minZ = std::numeric_limits<float>::max();
  maxX = maxY = maxZ = std::numeric_limits<float>::min();
  for (const Hit& hit: hits) {
    maxX = std::max(maxX, hit.x);
    maxY = std::max(maxY, hit.y);
    maxZ = std::max(maxY, hit.z);

    minX = std::min(minX, hit.x);
    minY = std::min(minY, hit.y);
    minZ = std::min(minY, hit.z);
  }
  info.maxX0 = maxX;
  info.minX0 = minX;
  info.maxY0 = maxY;
  info.minY0 = minY;
  info.maxDxOverDz = (maxX - minX) / (maxZ - minZ);
  info.minDxOverDz = - info.maxDxOverDz;
  info.maxDyOverDz = (maxY - minY) / (maxZ - minZ);
  info.minDyOverDz = - info.maxDyOverDz;
  return info;  
}
