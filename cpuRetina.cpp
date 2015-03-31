#include <cmath>
#include <iostream>
#include <algorithm>

#include "cpuRetina.h"
#include "grid.h"
#include "physics.h"
#include "Tools.h"

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

std::vector<TrackPure> restoreBestTracksFromResponces(
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

std::vector<double> cpuCalculateRetinaResponces(
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

std::vector<Track> findHitsForTracks(
  const std::vector<TrackPure>& tracks,
  const std::vector<Hit>& hits
)
{
  auto hits_copy = hits;
  std::vector<Track> extendedTracks;
  extendedTracks.reserve(tracks.size());
  for (const TrackPure& track: tracks)
  {
    std::nth_element(hits_copy.begin(), hits_copy.end(), hits_copy.begin() + MAX_TRACK_SIZE,
      [track](const Hit& a, const Hit& b)
      {
        return getDistanceFromTrackToHit(track, a) < getDistanceFromTrackToHit(track, b);
      });

    Track extended;
    //Add best fitted, but it can add extra
    for (int i = 0; i < MAX_TRACK_SIZE; ++i)
    {
      extended.addHit(hits_copy[i].id);
    }
    extendedTracks.push_back(extended);
  }
  return extendedTracks;
}

std::vector<TrackPure> retinaAlgorithm(
  const std::vector<Dimension>& dimensions,
  const std::vector<TrackPure>& grid,
  const std::vector<Hit>& hits,
  const double retina_sharpness_coeffecient
)
{
  const std::vector<double> responces = cpuCalculateRetinaResponces(grid, hits, retina_sharpness_coeffecient);
  return restoreBestTracksFromResponces(dimensions, grid, responces);
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
    const std::vector<Hit> hits = parseHitsFromInputAndNormalize(const_cast<const uint8_t*>(&(*input[i])[0]), input[i]->size());
    const std::vector<TrackPure> restored = retinaAlgorithm(dimensions, grid, hits, RETINA_SHARPNESS_COEFFICIENT);
    const std::vector<Track> tracksWithHits = findHitsForTracks(restored, hits);
    putTracksInOutputFormat(tracksWithHits, output[i]);
  }
  return 0;
}