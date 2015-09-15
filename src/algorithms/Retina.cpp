#include <cmath>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <cstdlib>
#include <fstream> 

#include "../optimizations/Grid.h"
#include "../optimizations/GridOptimization.h"
#include "../Tools.h"
#include "../RetinaCore/Definitions.cuh"
#include "Retina.h"
#include "Physics.h"
#include "HitsFinders.h"

#ifdef USE_CPU
#include "../RetinaCore/CpuRetina.h"
#else
#include "../RetinaCore/GpuRetina.cuh"
#endif

using namespace std::placeholders;

std::vector<TrackPure> retinaProjectionTrackRestore(
  const EventInfo& event,
  const IOptimization<TrackProjection>* optimization,
  double sharpness
)
{
  std::vector<double> hitsX(event.hits.size());
  std::vector<double> hitsY(event.hits.size());
  std::vector<double> hitsZ(event.hits.size());
  for (size_t i = 0; i < event.hits.size(); ++i)
  {
    hitsX[i] = event.hits[i].x;
    hitsY[i] = event.hits[i].y;
    hitsZ[i] = event.hits[i].z;
  }
  std::vector<double> valueCp;
  auto restoredDx = optimization->findMaximums(
    [&](const std::vector<TrackProjection>& tracks) -> std::vector<double>
    {
      std::vector<double> values(tracks.size());
#ifdef USE_CPU
        getRetina2dCpu(
#else
        getRetina2dGpu(
#endif
        tracks.data(),
        tracks.size(),
        hitsX.data(),
        hitsZ.data(),
        event.hits.size(),
        sharpness,
        values.data()
      );
      return values;
    }
  );
  auto restoredDy = optimization->findMaximums(
    [&](const std::vector<TrackProjection>& tracks) -> std::vector<double>
    {
      std::vector<double> values(tracks.size());
#ifdef USE_CPU
      getRetina2dCpu(
#else
      getRetina2dGpu(
#endif
        tracks.data(),
        tracks.size(),
        hitsY.data(),
        hitsZ.data(),
        event.hits.size(),
        sharpness,
        values.data()
      );
      return values;
    }
  );
  std::vector<TrackPure> tracks;
  
  std::vector<std::vector<std::pair<double, Hit> > > eventsDx = findBestHits<TrackProjection>(event, restoredDx, getDistanceDx);
  std::vector<std::vector<std::pair<double, Hit> > > eventsDy = findBestHits<TrackProjection>(event, restoredDy, getDistanceDy);
  std::vector<std::vector<int> > cleanedDx(eventsDx.size());
  for (int i = 0; i < eventsDx.size(); ++i)
  {
    for (int j = 0; j < eventsDx[i].size(); ++j)
    {
      if (eventsDx[i][j].first < PARAM_TOLERANCE)
      {
        cleanedDx[i].push_back(eventsDx[i][j].second.id);
      }
    }   
  }
  std::vector<std::vector<int> > cleanedDy(eventsDy.size());
  for (int i = 0; i < eventsDy.size(); ++i)
  {
    for (int j = 0; j < eventsDy[i].size(); ++j)
    {
      if (eventsDy[i][j].first < PARAM_TOLERANCE)
      {
        cleanedDy[i].push_back(eventsDy[i][j].second.id);
      }
    }   
  }
  
  for (const std::pair<int,int>& pair: setIntersection(cleanedDx, cleanedDy, 3))
  {
    
    TrackProjection& dx = restoredDx[pair.first];
    TrackProjection& dy = restoredDy[pair.second];
        
    tracks.push_back(TrackPure(dx.x0, dx.dx, dy.x0, dy.dx));
  }
  return tracks;
}

std::vector<TrackPure> retinaFullTrackRestore(
  const EventInfo& event,
  const IOptimization<TrackPure>* optimization,
  double sharpness
)
{
  auto tracks = optimization->findMaximums(
    [&](const std::vector<TrackPure>& tracks) -> std::vector<double>
    {
      std::vector<double> values(tracks.size());
#ifdef USE_CPU
        getRetina3dCpu(
#else
        getRetina3dGpu(
#endif
        tracks.data(),
        tracks.size(),
        event.hits.data(),
        event.hits.size(),
        sharpness,
        values.data()
      );
      return values;
    }
  );
  return tracks;
}

TrackProjection trackProjectionGenerator(const std::vector<double>& vector) {
  return TrackProjection(vector[0], vector[1]);
}

TrackPure trackPureGenerator(const std::vector<double>& vector) {
  return TrackPure(vector[0], vector[1], vector[2], vector[3]);
}
