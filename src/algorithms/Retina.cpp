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
TrackProjection trackProjectionGenerator(const std::vector<double>& vector) {
  return TrackProjection(vector[0], vector[1]);
}

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
//#define USE_CPU
#ifdef USE_CPU
  [&](TrackProjection track) -> double
  {
    double responce = 0;
    for (const Hit& hit : event.hits)
    {
      responce += exp(-getDistanceDx(track, hit) * sharpness);
    }
    return responce;
  }
#else
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
      valueCp = values;
      return values;
    }
#endif
  );
/*#ifndef USE_CPU
  for (size_t i = 0; i < grid.size(); ++i)
  {
    if ((fabs([&](TrackProjection track) -> double
    {
      double responce = 0;
      for (const Hit& hit : event.hits)
      {
        responce += exp(-getDistanceDx(track, hit) * sharpness);
      }
      return responce;
    }(grid.getPoint(i)) - valueCp[i]) > 1e-6)) 
      std::cerr << i << std::endl;
  }
#endif*/
  auto restoredDy = GridOptimization<TrackProjection>(grid).findMaximums(
#ifdef USE_CPU
  [&](TrackProjection track) -> double
    {
      double responce = 0;
      for (const Hit& hit : event.hits)
      {
        responce += exp(-getDistanceDy(track, hit) * sharpness);
      }
      return responce;
    }
#else
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
      valueCp = values;
      return values;
    }
#endif
  );
/*#ifndef USE_CPU
  for (size_t i = 0; i < grid.size(); ++i)
  {
    if ((fabs([&](TrackProjection track) -> double
    {
      double responce = 0;
      for (const Hit& hit : event.hits)
      {
        responce += exp(-getDistanceDy(track, hit) * sharpness);
      }
      return responce;
    }(grid.getPoint(i)) - valueCp[i]) > 1e-6)) 
      std::cerr << i << std::endl;
  }
#endif
  */
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

TrackPure trackPureGenerator(const std::vector<double>& vector) {
  return TrackPure(vector[0], vector[1], vector[2], vector[3]);
}

std::vector<TrackPure> retinaFullTrackRestore(
  const EventInfo& event,
  double sharpness
)
{
  const std::vector<std::vector<double> > dim = {
    generateUniformDimension(-1, 1, 30),
    generateUniformDimension(-1, 1, 30),
    generateUniformDimension(-0.3, 0.3, 30),
    generateUniformDimension(-0.3, 0.3, 30)
  };
  const std::vector<Hit>& hits = event.hits;
  Grid<TrackPure> grid(dim, trackPureGenerator);
  auto tracks = GridOptimization<TrackPure>(grid).findMaximums(
#define USE_CPU
#ifdef USE_CPU
    [&](TrackPure track) -> double
    {
      double responce = 0;
      for (const Hit& hit : hits)
      {
        responce += exp(-getDistance(track, hit) / sharpness);
      }
      return responce;
    }
#else
    [&](const std::vector<TrackPure>& tracks) -> std::vector<double>
    {
      std::vector<double> values(tracks.size());
      getRetina3dGpu(
        tracks.data(),
        tracks.size(),
        event.hits.data(),
        event.hits.size(),
        sharpness,
        values.data()
      );
      std::cerr << values[0] << std::endl;
      return values;
    }
#endif
  );
  std::cerr <<
    [&](TrackPure track) -> double
    {
      double responce = 0;
      for (const Hit& hit : hits)
      {
        responce += exp(-getDistance(track, hit) * sharpness);
      }
      return responce;
    }(grid.getPoint(0)) << std::endl;

  return tracks;
}
