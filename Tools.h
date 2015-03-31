#pragma once

#include <vector>
#include <cstdint>

#include "physics.h"

std::vector<Hit> parseHitsFromInputAndNormalize(const uint8_t * input, size_t size);

void putTracksInOutputFormat(
  const std::vector<Track>& tracks,
  std::vector<uint8_t>& output
);