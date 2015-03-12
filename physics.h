#pragma once
const double MIN_X0 = 0, MAX_X0 = 1;
const int GRID_SIZE_X0 = 3;

const double MIN_Y0 = 0, MAX_Y0 = 1;
const int GRID_SIZE_Y0 = 3;

const double MIN_XT = 0, MAX_XT = 1;
const int GRID_SIZE_XT = 3;

const double MIN_YT = 0, MAX_YT = 1;
const int GRID_SIZE_YT = 3;

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

inline double square(double x)
{
  return x * x;
}

double getDistanceFromTrackToHit(const TrackPure& track, const Hit& hit)
{
  return square(hit.x - track.x0 - track.tx * hit.z) + square(hit.y - track.y0 - track.ty * hit.z);
}

std::vector<Hit> parseHitsFromInput(uint8_t * input, size_t size)
{
  uint8_t * end = input + size;

  int h_no_sensors       = *((int32_t*)input); input += sizeof(int32_t);
  int h_no_hits          = *((int32_t*)input); input += sizeof(int32_t);
  int h_sensor_Zs        = *((int32_t*)input); input += sizeof(int32_t) * h_no_sensors;
  int h_sensor_hitStarts = *((int32_t*)input); input += sizeof(int32_t) * h_no_sensors;
  int h_sensor_hitNums   = *((int32_t*)input); input += sizeof(int32_t) * h_no_sensors;
  uint32_t* h_hit_IDs          = (uint32_t*)input; input += sizeof(uint32_t) * h_no_hits;
  float * h_hit_Xs           = (float*)  input; input += sizeof(float)   * h_no_hits;
  float * h_hit_Ys           = (float*)  input; input += sizeof(float)   * h_no_hits;
  float * h_hit_Zs           = (float*)  input; input += sizeof(float)   * h_no_hits;

  if (input != end)
    throw std::runtime_error("failed to deserialize event"); 
  
  std::vector<Hit> parsedHits(h_no_hits);
  
  for (int i = 0; i < h_no_hits; ++i)
  {
    parsedHits[i].id = h_hit_IDs[i];
    parsedHits[i].x = h_hit_Xs[i];
    parsedHits[i].y = h_hit_Ys[i];
    parsedHits[i].z = h_hit_Zs[i];
  }
  return parsedHits;
}

/* todo: put solutionTracks in output this way
   if (*num_tracks > 0) {
    solution.resize(*num_tracks * sizeof(Track));
    Track * solutionTracks = (Track*)&solution[0];
    for (size_t i = 0; i != *num_tracks; ++i)
      solutionTracks[i] = tracks[h_track_indexes[i]];
  }
*/





