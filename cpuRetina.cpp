#include <cmath>

#include "cpuRetina.h"
#include "grid.h"
#include "physics.h"
namespace {
  TrackPure generateTrackFromIndex(const std::vector<int>& indexes, const std::vector<Dimension>& dim)
  {
    return TrackPure(
      dim[0].getGridBoarder(indexes[0]),
      dim[1].getGridBoarder(indexes[1]),
      dim[2].getGridBoarder(indexes[2]),
      dim[3].getGridBoarder(indexes[3])
    );
  }

  std::vector<TrackPure> retinaRestores(const std::vector<Dimension>& dimensions, const std::vector<TrackPure>& grid, const std::vector<double>& responce)
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
      if (isLocalMaximum)
      {
        TrackPure answer = grid[currentIndex] * responce[currentIndex];
        double sum_responce = 0;
        for (int neighbour : neighbours)
        {
          answer = answer + grid[neighbour] * responce[neighbour];
          sum_responce += responce[neighbour];
        }
        restored.push_back(answer * (1.0 / sum_responce));      
      }
    }
    return restored;
  }

  std::vector<double> cpuCalculateRetinaResponces(const std::vector<TrackPure>& grid, const std::vector<Hit>& hits, double sharpness)
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
}
/**
 * Common entrypoint for Gaudi and non-Gaudi
 * @param input  
 * @param output 
 */
int cpuRetinaInvocation(
    const std::vector<const std::vector<uint8_t>* > & input,
    std::vector<std::vector<uint8_t> > & output) 
{
  std::vector<Dimension> dimensions = 
    { 
      Dimension(MIN_X_ON_Z0, MAX_X_ON_Z0, GRID_SIZE_X_ON_Z0),
      Dimension(MIN_Y_ON_Z0, MAX_Y_ON_Z0, GRID_SIZE_Y_ON_Z0),
      Dimension(MIN_DX_OVER_DZ, MAX_DX_OVER_DZ, GRID_SIZE_DX_OVER_DZ),
      Dimension(MIN_DY_OVER_DZ, MAX_DY_OVER_DZ, GRID_SIZE_DY_OVER_DZ)
    };
  auto grid = generateGrid<TrackPure>(dimensions, generateTrackFromIndex);
  for (unsigned int i = 0; i < input.size(); ++i)
  {
    auto in = *input[i];
    auto hits = parseHitsFromInput(const_cast<uint8_t*>(&in[0]), in.size());
    auto responces = cpuCalculateRetinaResponces(grid, hits, 1.0);
    auto restored = retinaRestores(dimensions, grid, responces);
    //output[i] = putGoodHits(restored, hits, minimalResponce);
  }
  return 0;
}