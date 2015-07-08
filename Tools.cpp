#include "Tools.h"

#include <map>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>

void setMinMax(double& min, double& max, const float * begin, int size)
{
  auto result = std::minmax_element(begin, begin + size);
  min = *result.first;
  max = *result.second;
}

//size_t findSensor(
//    const std::vector<double>& sensors, 
//    double position
//) {
//    auto larger = std::lower_bound(sensorsZs.begin(), sensorsZs.end(), position);
//    if (larger == sensors.end()) {
//        return sensors.size();
//    }
//    else if (larger == sensor.begin()) {
//        return 0;
//    }
//    else {
//        auto smaller = larger - 1;
//        if (fabs(*larger - position) < fabs(*smaller - postion)) {
//            return larger - sensors.begin();            
//        }
//        else {
//            return smaller - sensors.begin();
//        }
//    }
//}

std::vector<Hit> parseHits(const uint8_t * input, size_t size)
{
  const uint8_t * end = input + size;
  std::cerr << "everything is perfect" << std::endl;

  int h_no_sensors       = *((int32_t*)input); input += sizeof(int32_t);
  int h_no_hits          = *((int32_t*)input); input += sizeof(int32_t);
  int* h_sensor_Zs        = ((int32_t*)input); input += sizeof(int32_t) * h_no_sensors;
  int* h_sensor_hitStarts = ((int32_t*)input); input += sizeof(int32_t) * h_no_sensors;
  int* h_sensor_hitNums   = ((int32_t*)input); input += sizeof(int32_t) * h_no_sensors;
  uint32_t* h_hit_IDs          = (uint32_t*)input; input += sizeof(uint32_t) * h_no_hits;
  float * h_hit_Xs           = (float*)  input; input += sizeof(float)   * h_no_hits;
  float * h_hit_Ys           = (float*)  input; input += sizeof(float)   * h_no_hits;
  float * h_hit_Zs           = (float*)  input; input += sizeof(float)   * h_no_hits;

  if (input != end)
    throw std::runtime_error("failed to deserialize event"); 
  
  double minX, maxX, minY, maxY, minZ, maxZ;
  setMinMax(minX, maxX, h_hit_Xs, h_no_hits);
  setMinMax(minY, maxY, h_hit_Ys, h_no_hits);
  setMinMax(minZ, maxZ, h_hit_Zs, h_no_hits);
  
  std::vector<Hit> parsedHits;
  parsedHits.reserve(h_no_hits);
  /*std::vector<double> sensors;
  sensors.reserve(h_no_sensors);
  for (size_t i = 0; i < h_no_sensors; ++i) {
      sensors.push_back(h_sensor_Zs[i]);
  }
  for (int i = 0; i < h_no_hits; ++i)
  {
    parsedHits.push_back(Hit(
      (h_hit_Xs[i] - minX) / (maxX - minX),
      (h_hit_Ys[i] - minY) / (maxY - minY),
      (h_hit_Zs[i] - minZ) / (maxZ - minZ),
      h_hit_IDs[i],
      findSensor(h_sensor_Zs, h_no_sensors, h_hit_Zs[i])    
    ));
  }*/
  for (uint32_t sensorId =  0; sensorId < h_no_sensors; ++sensorId)
  {
    for (int i = 0; i < h_sensor_hitNums[sensorId]; ++i) 
    {
      int hitId = h_sensor_hitStarts[sensorId] + i;
      assert(hitId >= 0 && hitId < h_no_hits );
      parsedHits.emplace_back(
        (h_hit_Xs[hitId] - minX) / (maxX - minX),
        (h_hit_Ys[hitId] - minY) / (maxY - minY),
        (h_hit_Zs[hitId] - minZ) / (maxZ - minZ),
        h_hit_IDs[hitId],
        sensorId
      );
      std::cerr << parsedHits.rbegin()->sensorId << std::endl;
    }
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

void printHit(const Hit& hit, std::ostream& stream) 
{
  //const int hitNumber = track.hits[i];  todo: Why do we need this information
  const int hitNumber = hit.id; 
  
  stream << " " << std::setw(8) << hit.id << " (" << hitNumber << ")"
    << " module " << std::setw(2) << hit.sensorId
    << ", x " << std::setw(6) << hit.x
    << ", y " << std::setw(6) << hit.y
    << ", z " << std::setw(6) << hit.z 
    << std::endl;
  
}

void printSolution(
  const std::vector<Track>& tracks, 
  const std::vector<Hit>& hits,
  std::ostream& stream
)
{
  std::map<uint32_t, Hit> hitMap;
  for (const Hit& hit: hits) {
    hitMap[hit.id] = hit;
    assert(hitMap[hit.id].sensorId == hit.sensorId);
  }
  for (size_t trackId = 0; trackId < tracks.size(); ++trackId) 
  {
    const Track& track = tracks[trackId];
    stream << "Track #" << trackId << ", length " << track.hitsNum << std::endl;
    for(int i = 0; i < track.hitsNum; ++i)
    {
      std::cerr << hitMap[track.hits[i]].sensorId << std::endl;
      printHit(hitMap[track.hits[i]], stream);
    }
  }

  stream << std::endl;
}

