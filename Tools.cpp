#include "Tools.h"

#include <algorithm>
#include <iostream>

void outMinMax(const std::string& name, const float * begin, int size)
{
  std::cerr << name << " " << *std::min_element(begin, begin + size) 
    << " " << *std::max_element(begin, begin + size) << std::endl;
  
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
  outMinMax("X", h_hit_Xs, h_no_hits);
  outMinMax("Y", h_hit_Ys, h_no_hits);
  outMinMax("Z", h_hit_Zs, h_no_hits);
  std::vector<Hit> parsedHits;
  parsedHits.reserve(h_no_hits);
  for (int i = 0; i < h_no_hits; ++i)
  {
    parsedHits.push_back(Hit(
      h_hit_Xs[i],
      h_hit_Ys[i],
      h_hit_Zs[i],
      h_hit_IDs[i]
    ));
  }
  return parsedHits;
}

void putTracksInOutputFormat(
  const std::vector<Track>& tracks,
  std::vector<uint8_t>& output
)
{
  output = std::vector<uint8_t>(tracks.size() * sizeof(Track));
  Track * outputPtr = (Track*)&output[0];
  for (size_t i = 0; i != tracks.size(); ++i)
    outputPtr[i] = tracks[i];
}
