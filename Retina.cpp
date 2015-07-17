#include <cmath>
#include <iostream>
#include <algorithm>
#include <map>

#include "Retina.h"
#include "Grid.h"
#include "Physics.h"
#include "Tools.h"
#include "Logger.h"

TrackPure generateTrackFromIndex(
  const std::vector<int>& multiIndex,
  const std::vector<Dimension>& dim
)
{
  return TrackPure(
    dim[0].getGridBoarder(multiIndex[0]),
    dim[1].getGridBoarder(multiIndex[1]),
    dim[2].getGridBoarder(multiIndex[2]),
    dim[3].getGridBoarder(multiIndex[3])
  );
}

std::vector<TrackPure> restoreTracks(
  const std::vector<Dimension>& dimensions,
  const std::vector<TrackPure>& grid,
  const std::vector<double>& responces
)
{
  int gridSizeNoBoarders = calculateGridSize(dimensions, 1);
  std::vector<int> indexes(dimensions.size(), 1);
  std::vector<TrackPure> restored;
  const double threshold = getQuatile(responces, TRACK_THRESHOLD);
  for (int i = 0; i < gridSizeNoBoarders; ++i)
  {
    std::vector<int> neighbours = generateNeighboursIndexes(indexes, dimensions);
    int currentIndex = multiIndexToIndex(indexes, dimensions);
    if (responces[currentIndex] > threshold)
    {
      TrackPure answer = grid[currentIndex] * responces[currentIndex];
      double sum_responce = responces[currentIndex];
      for (int neighbour : neighbours)
      {
        answer = answer + grid[neighbour] * responces[neighbour];
        sum_responce += responces[neighbour];
      }
      restored.push_back(answer * (1.0 / sum_responce));      
    }
    generateNextMultiIndex(indexes, dimensions, 1);
  }
  return restored;
}

std::vector<double> calculateResponses(
  const std::vector<TrackPure>& grid,
  const std::vector<Hit>& hits,
  double sharpness
)
{
  std::vector<double> responces(grid.size());
  for (unsigned int i = 0; i < grid.size(); ++i) 
  {
    auto& track = grid[i];
    for (const Hit& hit : hits)
    {
      responces[i] += exp(-getDistance(track, hit) / sharpness);
    }
  }
  return responces;
}

std::vector<Track> findHits(
  const std::vector<TrackPure>& tracks,
  const std::vector<Hit>& hits
)
{
  std::vector<Track> extendedTracks;
  extendedTracks.reserve(tracks.size());
  for (const TrackPure& track: tracks)
  {
    std::map<uint32_t, Hit> sensorsBest;
    for (const Hit& hit: hits) 
    {
      if (!sensorsBest.count(hit.sensorId) ||
          getDistance(track, sensorsBest[hit.sensorId]) > 
          getDistance(track, hit))
      {
        sensorsBest[hit.sensorId] = hit;
      }
    }
    Track extended;
    for (const auto& pair: sensorsBest)
    {
      if (getDistance(track, pair.second) < HIT_THRESHOLD)
      {
        extended.addHit(pair.second.id);
      }
    }
    extendedTracks.push_back(extended);
  }
  return extendedTracks;
}

/**
 * Common entrypoint for Gaudi and non-Gaudi
 * @param input  
 * @param output 
 */
int cpuRetinaInvocation(
    const std::vector<const std::vector<uint8_t>* > & input,
    std::vector<std::vector<uint8_t> > & output
)
{
  const std::vector<Dimension> dimensions =
    {
      Dimension(-1, 2, GRID_SIZE_X_ON_Z0),
      Dimension(-1, 2, GRID_SIZE_Y_ON_Z0),
      Dimension(-2, 2, GRID_SIZE_DX_OVER_DZ),
      Dimension(-2, 2, GRID_SIZE_DY_OVER_DZ)
    };
  const std::vector<TrackPure> grid = generateGrid<TrackPure>(dimensions, generateTrackFromIndex);
 
  output.resize(input.size());
  for (size_t i = 0; i < input.size(); ++i)
  {
    const std::vector<Hit> hits = parseHits(const_cast<const uint8_t*>(&(*input[i])[0]), input[i]->size());
    const std::vector<double> responses = calculateResponses(grid, hits, RETINA_SHARPNESS_COEFFICIENT);
    const std::vector<TrackPure> restored = restoreTracks(dimensions, grid, responses);
    const std::vector<Track> tracksWithHits = findHits(restored, hits);
    putTracksInOutputFormat(tracksWithHits, output[i]);
    printSolution(tracksWithHits, hits, DEBUG);
  }
  return 0;
}
