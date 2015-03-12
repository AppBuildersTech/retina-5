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
vector<TrackN> main(const vector<Hit>& hits)
{
}

std::vector<double> cpuCalculateRetinaResponce(const std::vector<Receptor>& receptors, const std::vector<Hit>& hits, double sharpness)
{
  std::vector<double> responces;
  for (Receptor receptor : receptors) 
  {
    double sum = 0;
    for (const Hit& hit : hits)
    {
      sum += exp(-getDistanceFromReceptorToHit(receptor, hit) / sharpness);
    }
    responces.push_back(sum);
  }
  return responces;
}*/
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
  std::cerr << grid.size() << std::endl;
  for (auto x : grid)
  {
    std::cerr << x.x0 << " " << x.y0 << " " << x.tx << " " << x.ty << std::endl;
  }
  //auto responce = calculateRetinaResponse(grid, hits);
  //auto restored = retinaRestores(demenison, responce);
  //output = putGoodHits(restored, hits, minimalResponce);
  return 0;
}