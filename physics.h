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

/*double distance(const TrackPure& track, const Hit& hit)
{
  return square(hit.x - track.x - tx * hit.z) + square(hit.y - track.y - ty * hit.z);
}*/






