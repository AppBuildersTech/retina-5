#include <cmath>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <cstdlib>
#include <fstream> 

#include "optimizations/GridOptimization.h"
#include "optimizations/Grid.h"
#include "Retina.h"
#include "Physics.h"

std::vector<std::pair<double, Hit> > findHitsOnSensor(  
  const std::vector<Hit>& hits,
  const TrackPure& track,
  const std::function<double(TrackPure,Hit)> distance
)
{
  std::vector<std::pair<double, Hit> > distances;
  std::map<uint32_t, Hit> sensorsBest;

  for (size_t i = 0; i < hits.size(); ++i)
  {
    {
      const Hit& hit = hits[i];
      if (!sensorsBest.count(hit.sensorId) ||
          (distance(track, sensorsBest[hit.sensorId]) > 
          distance(track, hit)))
      {
        sensorsBest[hit.sensorId] = hit;
      }
    }
  }
  for (const auto& pair: sensorsBest)
  {
    distances.emplace_back(getDistance(track, pair.second), pair.second);
  }
  return distances;
}

std::vector<Track> findHits(
  const std::vector<TrackPure>& tracks,
  const std::vector<Hit>& hits
)
{
  std::vector<Track> extendedTracks;
  extendedTracks.reserve(tracks.size());
  std::set<std::vector<uint32_t> > tracksSet;
  std::vector<bool> used(hits.size(), false);
  int cnt = 0;
  for (const TrackPure& track: tracks)
  {
    if (++cnt % 1000 == 0)
    {
      std::cerr << cnt << " tracks proceed" << std::endl;
    }
    std::vector<std::pair<double, Hit> > distances = findHitsOnSensor(hits, track, getDistance) ;
    Track extended;
    std::sort(distances.begin(), distances.end(), 
      [](const std::pair<double, Hit>& a, const std::pair<double, Hit>& b) -> bool
      {
        return a.first < b.first;
      }
    );
    {
      double zStart = distances[0].second.z;
      extended.addHit(distances[0].second.id);
      for (size_t i = 1; i < 3; /*distances.size()*/ i++)
      {
        if (isFit(track, distances[i].second, zStart))
        {
          extended.addHit(distances[i].second.id);
        }
      }
    }
    if (extended.hitsNum > 2)
    {
      /*for (size_t i = 0; i < extended.hitsNum; ++i)
      {
        used[extended.hits[i]] = true;
      }*/
      extendedTracks.push_back(extended);
    }
  }
        
  return extendedTracks;
}

void outputBest(
  const std::vector<TrackPure>& tracks,
  const std::vector<Hit>& hits,
  const std::function<double(TrackPure,Hit)> distance,
  std::string name
)
{
  std::ofstream myfile;
  myfile.open (name);
  for (const TrackPure& track: tracks)
  {
    auto bst = findHitsOnSensor(hits, track, distance);
    for (auto p: bst)
    {
      myfile << p.second.id << " ";
    }
    myfile << std::endl;
  }
  myfile.close();
}

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
std::vector<Track> algorithm(const EventInfo& event)
{
  const std::vector<std::vector<double> > dim = {
    generateUniformDimension(-1, 1, 20),
    generateUniformDimension(-1, 1, 20),
    generateUniformDimension(-0.3, 0.3, 20),
    generateUniformDimension(-1, 1, 20)
  };
  const std::vector<Hit>& hits = event.hits;
  //auto answer = findHits;
  //output[i] = answer;
  //printSolution(tracksWithHits, hits, DEBUG);
  return std::vector<Track>();
}
