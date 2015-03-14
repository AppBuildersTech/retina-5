#include "physics.h"

TrackPure operator*(const TrackPure& one, const double alpha)
{
  TrackPure answer(one);
  answer.x0 *= alpha;
  answer.y0 *= alpha;
  answer.tx *= alpha;
  answer.ty *= alpha;
  return answer;
}

TrackPure operator+(const TrackPure& one, const TrackPure& other) 
{
  TrackPure answer(one);
  answer.x0 += other.x0;
  answer.y0 += other.y0;
  answer.tx += other.tx;
  answer.ty += other.ty;
  return answer;
}

std::vector<Hit> parseHitsFromInput(uint8_t * input, size_t size)
{
  uint8_t * end = input + size;

  int h_no_sensors       = *((int32_t*)input); input += sizeof(int32_t);
  int h_no_hits          = *((int32_t*)input); input += sizeof(int32_t);
  int h_sensor_Zs        = *((int32_t*)input); input += sizeof(int32_t) * h_no_sensors;
  int h_sensor_hitStarts = *((int32_t*)input); input += sizeof(int32_t) * h_no_sensors;
  int h_sensor_hitNums   = *((int32_t*)input); input += sizeof(int32_t) * h_no_sensors;
  uint32_t* h_hit_IDs          = (uint32_t*)input; input += sizeof(uint32_t) * h_no_hits;
  float * h_hit_Xs           = (float*)  input; input += sizeof(float)   * h_no_hits;
  float * h_hit_Ys           = (float*)  input; input += sizeof(float)   * h_no_hits;
  float * h_hit_Zs           = (float*)  input; input += sizeof(float)   * h_no_hits;

  if (input != end)
    throw std::runtime_error("failed to deserialize event"); 
  
  std::vector<Hit> parsedHits(h_no_hits);
  for (int i = 0; i < h_no_hits; ++i)
  {
    parsedHits[i].id = h_hit_IDs[i];
    parsedHits[i].x = h_hit_Xs[i];
    parsedHits[i].y = h_hit_Ys[i];
    parsedHits[i].z = h_hit_Zs[i];
  }
  return parsedHits;
}

inline double square(double x)
{
  return x * x;
}

double getDistanceFromTrackToHit(const TrackPure& track, const Hit& hit)
{
  return square(hit.x - track.x0 - track.tx * hit.z) + square(hit.y - track.y0 - track.ty * hit.z);
}
