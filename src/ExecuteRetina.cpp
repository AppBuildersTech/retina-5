#include <iostream>
#include <functional>
#include <memory>

#include "ExecuteRetina.h"
#include "algorithms/Retina.h"
#include "Tools.h"
#include "optimizations/GridOptimization.h"


/**
 * Common entrypoint for Gaudi and non-Gaudi
 * @param input  
 * @param output 
 */
int cpuRetinaInvocation(
   const std::vector<const std::vector<uint8_t>* >  & input,
  std::vector<std::vector<uint8_t> > & output
) {
  /*const std::vector<std::vector<double> > dim = {
    generateUniformDimension(-1, 1, 30),
    generateUniformDimension(-1, 1, 30),
    generateUniformDimension(-0.3, 0.3, 30),
    generateUniformDimension(-0.3, 0.3, 30)
  };
  Grid<TrackPure> grid(dim, trackPureGenerator);
  std::unique_ptr<GridOptimization<TrackPure> > optimization(new GridOptimization<TrackPure>(grid));
  */
  const std::vector<std::vector<double> > dim = {
    generateUniformDimension(-1, 1, 1000),
    generateUniformDimension(-0.3, 0.3, 1000)
  };
  Grid<TrackProjection> grid(dim, trackProjectionGenerator);
  std::unique_ptr<GridOptimization<TrackProjection> > optimization(new GridOptimization<TrackProjection>(grid));

  //auto findTracks = std::bind(retinaFullTrackRestore, std::placeholders::_1, ptr, 1e3);
  auto findTracks = std::bind(retinaProjectionTrackRestore, std::placeholders::_1, optimization.get(), 1e3);
  auto findPoints = std::bind(findHits, std::placeholders::_1, std::placeholders::_2);

  output.resize(input.size());
  for (size_t i = 0; i < input.size(); ++i)
  {
    const EventInfo event = parseEvent(
      const_cast<const uint8_t*>(input[i]->data()),
      input[i]->size()
    );    
    
    std::vector<TrackPure> pureTracks = findTracks(event);
    auto tracks = findPoints(pureTracks, event);
    output[i] = putTracksInOutputFormat(event.hits, tracks);
  }
  return 0;
}
int independent_execute(
    const std::vector<std::vector<uint8_t> > & input,
    std::vector<std::vector<uint8_t> > & output
) {

  std::vector<const std::vector<uint8_t>* > converted_input;
  converted_input.resize(input.size());

  for (int i = 0; i < (int)input.size(); ++i) {
    converted_input[i] = &(input[i]);
  }

  return cpuRetinaInvocation(converted_input, output);
}

void independent_post_execute(const std::vector<std::vector<uint8_t> > & output) {
    std::cout << "post_execute invoked" << std::endl;
    for (size_t i = 0; i < output.size(); ++i)
      std::cout << "Size of output[" <<  i << "]: " << output[i].size() << " B" << std::endl;
}
