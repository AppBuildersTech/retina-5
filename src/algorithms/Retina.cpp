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
#include "../RetinaCore/Definitions.cuh"
#include "../RetinaCore/GpuRetina.cuh"
#include "Retina.h"
#include "Physics.h"
#include "HitsFinders.h"

using namespace std::placeholders;

std::vector<TrackPure> retinaProjectionTrackRestore(
  const EventInfo& event,
  double sharpness
)
{
  const std::vector<std::vector<double> > dim = {
    generateUniformDimension(-1, 1, 1000),
    generateUniformDimension(-0.03, 0.03, 1000)
  };
  Grid<TrackProjection> grid(dim, trackProjectionGenerator);
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
  auto restoredDx = GridOptimization<TrackProjection>(grid).findMaximums(
    [&](const std::vector<TrackProjection>& tracks) -> std::vector<double>
    {
      std::vector<double> values(tracks.size());
      getRetina2dGpu(
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
  auto restoredDy = GridOptimization<TrackProjection>(grid).findMaximums(
    [&](const std::vector<TrackProjection>& tracks) -> std::vector<double>
    {
      std::vector<double> values(tracks.size());
      getRetina2dGpu(
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
  for (size_t x = 0; x < eventsDx.size(); ++x)
  {
    std::vector<std::pair<double, Hit> >& xHits = eventsDx[x];
    for (size_t y = 0; y < eventsDy.size(); ++y)
    {
      std::vector<std::pair<double, Hit> >& yHits = eventsDy[y];
      int intersection = 0;

      for (int j = 0; j < xHits.size(); j++)
      {
        if (xHits[j].first + yHits[j].first < PARAM_TOLERANCE && xHits[j].second.id == yHits[j].second.id)
        {
          intersection++;
        }
      }
      if (intersection > 2)
      {
        TrackProjection& dx = restoredDx[x];
        TrackProjection& dy = restoredDy[y];
        
        tracks.push_back(TrackPure(dx.x0, dx.dx, dy.x0, dy.dx));
      }
    }
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
