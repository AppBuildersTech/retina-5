#include <cmath>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <cstdlib>
#include <fstream> 

#include "../optimizations/Grid.h"
#include "../optimizations/GridOptimization.h"
#include "Retina.h"
#include "Physics.h"
#include "HitsFinders.h"

TrackProjection trackProjectionGenerator(const std::vector<double>& vector) {
  return TrackProjection(vector[0], vector[1]);
}

std::vector<TrackPure> retinaProjectionTrackRestore(
  const EventInfo& event,
  double sharpness
)
{
  const std::vector<std::vector<double> > dim = {
    generateUniformDimension(-1, 1, 500),
    generateUniformDimension(-0.03, 0.03, 500)
  };
  Grid<TrackProjection> grid(dim, trackProjectionGenerator);

  auto restoredDx = GridOptimization<TrackProjection>(grid).findMaximums(
    [&](TrackProjection track) -> double
    {
      double responce = 0;
      for (const Hit& hit : event.hits)
      {
        responce += exp(-getDistanceDx(track, hit) / sharpness);
      }
      return responce;
    }
  );
  
  auto restoredDy = GridOptimization<TrackProjection>(grid).findMaximums(
    [&](TrackProjection track) -> double
    {
      double responce = 0;
      for (const Hit& hit : event.hits)
      {
        responce += exp(-getDistanceDy(track, hit) / sharpness);
      }
      return responce;
    }
  );
  std::vector<TrackPure> tracks;
  for (const TrackProjection& dx : restoredDx)
  {
    std::vector<std::pair<double, Hit> > xHits = findBestHits(event, std::bind(getDistanceDx, dx, std::placeholders::_1));
    for (const TrackProjection& dy: restoredDy)
    {
      std::vector<std::pair<double, Hit> > yHits = findBestHits(event, std::bind(getDistanceDy, dy, std::placeholders::_1));
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
    generateUniformDimension(-1, 1, 10),
    generateUniformDimension(-1, 1, 10),
    generateUniformDimension(-0.3, 0.3, 10),
    generateUniformDimension(-0.3, 0.3, 10)
  };
  const std::vector<Hit>& hits = event.hits;
  Grid<TrackPure> grid(dim, trackPureGenerator);
  auto tracks = GridOptimization<TrackPure>(grid).findMaximums(
    [&](TrackPure track) -> double
    {
      double responce = 0;
      for (const Hit& hit : hits)
      {
        responce += exp(-getDistance(track, hit) / sharpness);
      }
      return responce;
    }
  );
  return tracks;
}
