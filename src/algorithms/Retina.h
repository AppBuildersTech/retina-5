#pragma once

#include <vector>
#include <stdint.h>

#include "../Data.h"
#include "Physics.h"
#include "../optimizations/GridOptimization.h"


std::vector<TrackPure> retinaProjectionTrackRestore(
  const EventInfo& event,
  double sharpness
);

std::vector<TrackPure> retinaFullTrackRestore(
  const EventInfo& event,
  const IOptimization<TrackPure>* optimization,
  double sharpness
);

std::vector<Track> findHits(
  const std::vector<TrackPure>& tracks,
  const EventInfo& event
);

TrackProjection trackProjectionGenerator(const std::vector<double>& vector);

TrackPure trackPureGenerator(const std::vector<double>& vector);
