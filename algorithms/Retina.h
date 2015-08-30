#pragma once

#include <vector>
#include <stdint.h>

#include "../Data.h"
#include "Physics.h"

std::vector<TrackPure> retinaProjectionTrackRestore(
  const EventInfo& event,
  double sharpness
);

std::vector<TrackPure> retinaFullTrackRestore(const EventInfo& event, double sharpness);

std::vector<Track> findHits(
  const std::vector<TrackPure>& tracks,
  const EventInfo& event
);

