/* 
 * File:   GpuRetina.cuh
 * Author: towelenee
 *
 * Created on August 30, 2015, 8:43 PM
 */
#include "Physics.h"
#include "../Data.h"
#include <vector>
#pragma once

std::vector<double> getRetinaDx(const std::vector<TrackProjection>& tracks, const EventInfo& event, double sharpness);
