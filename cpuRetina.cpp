#include "cpuRetina.h"
#include "grid.h"
#include "physics.h"
#include <cmath>

int independent_execute(
    const std::vector<std::vector<uint8_t> > & input,
    std::vector<std::vector<uint8_t> > & output) {

  std::vector<const std::vector<uint8_t>* > converted_input;
  converted_input.resize(input.size());

  for (int i = 0; i < (int)input.size(); ++i) {
    converted_input[i] = &(input[i]);
  }

  std::cout << std::fixed << std::setprecision(2);
  logger::ll.verbosityLevel = 3;

  return cpuRetinaInvocation(converted_input, output);
}

void independent_post_execute(const std::vector<std::vector<uint8_t> > & output) {
    DEBUG << "post_execute invoked" << std::endl;
    DEBUG << "Size of output: " << output.size() << " B" << std::endl;
}

int cpuRetina(
    const std::vector<const std::vector<uint8_t>* > & input,
    std::vector<std::vector<uint8_t> > & output) {

  // Silent execution
  std::cout << std::fixed << std::setprecision(2);
  logger::ll.verbosityLevel = 3;
  return cpuRetinaInvocation(input, output);
}



TrackPure generateTrackFromIndex(const std::vector<int>& indexes, const std::vector<Dimension>& dim)
{
  TrackPure ans;
  ans.x0 = dim[0].get(indexes[0]);
  ans.y0 = dim[1].get(indexes[1]);
  ans.tx = dim[2].get(indexes[2]);
  ans.ty = dim[3].get(indexes[3]);
  return ans;
}

/*
vector<Track> retinaRestores(const vector<Dimension>& dimensions, const vector<double> responce)
{
  int gridSizeNoBoarders = calculateGridSize(dimensions, 1);
  vector<int> indexes(dimensions.size(), 1);
  for (int i = 0; i < gridSizeNoBoarders; ++i)
  {
    vector<int> neighbours = generateNeighboursIndexes(index, dimensions);
    int currentIndex = multiIndexToIndex(index, dimensions);
    //checkLocalMaximum
      //Restore
  }

}
*/
std::vector<double> cpuCalculateRetinaResponce(const std::vector<TrackPure>& grid, const std::vector<Hit>& hits, double sharpness)
{
  std::vector<double> responces(grid.size());
  for (unsigned int i = 0; i < grid.size(); ++i) 
  {
    auto& track = grid[i];
    for (const Hit& hit : hits)
      responces[i] += exp(-getDistanceFromTrackToHit(track, hit) / sharpness);
  }
  return responces;
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
    for (Hit& hit : hits)
    {
      std::cerr << hit.x << " " << hit.y << " " << hit.z << " " << hit.id << std::endl;
    }
    //auto responce = cpuCalculateRetinaResponce(grid, hits, 1);
    //auto restored = retinaRestores(demenison, responce);
    //output[i] = putGoodHits(restored, hits, minimalResponce);
  }
  return 0;
}