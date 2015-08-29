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

/* Join tracks
    for (const TrackPure& dx : restoredDx)
    {
      std::vector<std::pair<double, Hit> > xHits = findHitsOnSensor(hits, dx, getDistance) ;
      for (const TrackPure& dy: restoredDy)
      {
        std::vector<std::pair<double, Hit> > yHits = findHitsOnSensor(hits, dx, getDistance) ;
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
          tracks.push_back(dx + dy);
        }
      }
    }
*/

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
