#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

const double MIN_X0 = -100, MAX_X0 = 100;
const int GRID_SIZE_X0 = 5;

const double MIN_Y0 = -100, MAX_Y0 = 100;
const int GRID_SIZE_Y0 = 5;

const double MIN_XT = -100, MAX_XT = 100;
const int GRID_SIZE_XT = 5;

const double MIN_YT = -100, MAX_YT = 100;
const int GRID_SIZE_YT = 5;

const int MAX_TRACK_SIZE = 24;

struct Hit {
  float x;
  float y;
  float z;
  uint32_t id;
};

struct Track {
  float x0;
  float tx;
  float y0;
  float ty;

  int hitsNum;
  int hits[MAX_TRACK_SIZE];
};

struct TrackPure {
  float x0;
  float tx;
  float y0;
  float ty;
};

TrackPure operator*(const TrackPure& one, const double alpha);

TrackPure operator+(const TrackPure& one, const TrackPure& other);


double getDistanceFromTrackToHit(const TrackPure& track, const Hit& hit);

std::vector<Hit> parseHitsFromInput(uint8_t * input, size_t size);

/* todo: put solutionTracks in output this way
   if (*num_tracks > 0) {
    solution.resize(*num_tracks * sizeof(Track));
    Track * solutionTracks = (Track*)&solution[0];
    for (size_t i = 0; i != *num_tracks; ++i)
      solutionTracks[i] = tracks[h_track_indexes[i]];
  }
*/





