#include <cmath>

#include "cpuRetina.h"
#include "grid.h"
#include "physics.h"
namespace {
  TrackPure generateTrackFromIndex(const std::vector<int>& indexes, const std::vector<Dimension>& dim)
  {
      TrackPure ans;
      ans.x0 = dim[0].get(indexes[0]);
      ans.y0 = dim[1].get(indexes[1]);
      ans.tx = dim[2].get(indexes[2]);
      ans.ty = dim[3].get(indexes[3]);
      return ans;
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
      Dimension(MIN_X0, MAX_X0, GRID_SIZE_X0),
      Dimension(MIN_Y0, MAX_Y0, GRID_SIZE_Y0),
      Dimension(MIN_XT, MAX_XT, GRID_SIZE_XT),
      Dimension(MIN_YT, MAX_YT, GRID_SIZE_YT)
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