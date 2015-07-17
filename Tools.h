#pragma once

#include <vector>
#include <cstdint>

#include "Physics.h"

std::vector<Hit> parseHits(const uint8_t * input, size_t size);

void putTracksInOutputFormat(
  const std::vector<Track>& tracks,
  std::vector<uint8_t>& output
);

void printSolution(
  const std::vector<Track>& tracks, 
  const std::vector<Hit>& hits,
  std::ostream& stream
); 

double getQuatile(std::vector<double> data, double ratio);

