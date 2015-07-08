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
  const std::vector<double>& responce
)
{
  int gridSizeNoBoarders = calculateGridSize(dimensions, 1);
  std::vector<int> indexes(dimensions.size(), 1);
  std::vector<TrackPure> restored;
  for (int i = 0; i < gridSizeNoBoarders; ++i)
  {
    std::vector<int> neighbours = generateNeighboursIndexes(indexes, dimensions);
    int currentIndex = multiIndexToIndex(indexes, dimensions);
    bool isLocalMaximum = true;
    for (int neighbour : neighbours)
    {
      if (responce[neighbour] > responce[currentIndex])
      {
        isLocalMaximum = false;
      }
    }
    if (responce[currentIndex] > 1e-9 && isLocalMaximum)
    {
      TrackPure answer = grid[currentIndex] * responce[currentIndex];
      double sum_responce = responce[currentIndex];
      for (int neighbour : neighbours)
      {
        answer = answer + grid[neighbour] * responce[neighbour];
        sum_responce += responce[neighbour];
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
      responces[i] += exp(-getDistanceFromTrackToHit(track, hit) / sharpness);
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
    std::map<uint32_t, Hit> sensorBest;
    for (const Hit& hit: hits) 
    {
      if (!sensorBest.count(hit.sensorId) ||
          getDistanceFromTrackToHit(track, sensorBest[hit.sensorId]) < 
          getDistanceFromTrackToHit(track, hit))
      {
        sensorBest[hit.sensorId] = hit;
      }
    }
    Track extended;
    for (const auto& pair: sensorBest)
    {
      extended.addHit(pair.second.id);
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
    for (const Hit& hit : hits)
    {
      std::cerr << hit.sensorId << std::endl;
    }
    const std::vector<double> responses = calculateResponses(grid, hits, RETINA_SHARPNESS_COEFFICIENT);
    const std::vector<TrackPure> restored = restoreTracks(dimensions, grid, responses);
    const std::vector<Track> tracksWithHits = findHits(restored, hits);
    putTracksInOutputFormat(tracksWithHits, output[i]);
    printSolution(tracksWithHits, hits, DEBUG);
  }
  return 0;
}