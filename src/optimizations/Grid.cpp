#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <boost/math/distributions/normal.hpp>

#include "Grid.h"

std::vector<double> generateUniformDimension(double min, double max, size_t size)
{
  std::vector<double> dimension(size + 2);
  double quant = (max - min) / (size - 1);
  dimension[0] =  min - quant;
  for(size_t i = 0; i <= size; i++)
  {
    dimension[i + 1] = dimension[i] + quant;
  }
  return std::move(dimension);
}

std::vector<double> generateGaussDimension(double mean, double sd, size_t size)
{
  std::vector<double> dimension(size);
  boost::math::normal dist(mean, sd);
  double quant = 1. / size;
  for (size_t i = 1; i < size; i++)
  {
    dimension[i] = quantile(dist, quant * i);
  }
  std::sort(dimension.begin(), dimension.end());
  return std::move(dimension);
}
