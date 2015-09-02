#pragma once

#include "../Data.h"

const double RETINA_SHARPNESS_COEFFICIENT = 1e-3;
const double PARAM_TOLERANCE = 0.6;
const double SCATTER_TOLERANCE = 0.000016;

        
TrackPure operator*(const TrackPure& one, const double alpha);

TrackPure operator+(const TrackPure& one, const TrackPure& other);

TrackProjection operator*(const TrackProjection& one, const double alpha);

TrackProjection operator+(const TrackProjection& one, const TrackProjection& other);

double getDistance(const TrackPure& track, const Hit& hit) noexcept;
double getDistanceDx(const TrackProjection& track, const Hit& hit) noexcept;
double getDistanceDy(const TrackProjection& track, const Hit& hit) noexcept;

bool isFit(const TrackPure& track, const Hit& hit, double zStart) noexcept;