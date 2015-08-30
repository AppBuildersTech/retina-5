#include <cmath>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <cstdlib>
#include <fstream> 

#include "Physics.h"
#include "HitsFinders.h"

#include "GpuRetina.cuh"

std::vector<double> getRetinaDx(const std::vector<TrackProjection>& tracks, const EventInfo& event, double sharpness);
{
  std::vector<double> values(tracks.size());
  size_t tracksNum = tracks.size();
  const std::vector<Hit>& hits = event.hits;
  size_t hitsNum = hits.size();
  
  for (size_t trackId = 0; trackId < tracksNum; ++trackId)
  {
    double sum = 0;
    for (size_t hitId = 0; hitId < hitsNum; hitId++)
    {
      sum += exp(-getDistanceDx(tracks[trackId], hits[hitId]) / sharpness);
    }
    values[trackId] = sum;
  }
  return values;
}


