/* 
 * File:   GridOptimization.h
 * Author: towelenee
 *
 * Created on August 28, 2015, 6:53 PM
 */

#pragma once
#include "Grid.h"
#include "IOptimization.h"

template<class Arg>
class GridOptimization : IOptimization<Arg>
{
public:
  GridOptimization(const Grid<Arg>& grid) : grid(grid)
  {
  }
  
  std::vector<Arg> findMaximums(
    const std::function<double(Arg)> function
  )
  {
    std::vector<double> values;
    values.reserve(grid.size());
    for (size_t i = 0; i < grid.size(); ++i)
    {
      values.push_back(function(grid.getPoint(i)));
    }
    return restoreMaxims(values);  
  }
  
private:
  const Grid<Arg> grid;
  std::vector<Arg> restoreMaxims(
    const std::vector<double>& values
  )
  {
    std::vector<Arg> maxims;

    for (int i = 0; i < grid.size(); ++i)
    {
      std::vector<int> neighbours = grid.getNeighbours(i);
      if (neighbours.size() != 2 * grid.dim())
      {
        continue;
      }
      bool isLocalMaximum = true;
      for (int neighbour : neighbours)
      {
        if (values[neighbour] > values[i] + 1e-8)
        {
          isLocalMaximum = false;
        }
      }
      if (isLocalMaximum && values[i] > 1e-5)
      {
        Arg answer = grid.getPoint(i) * values[i];
        double sum_responce = values[i];
        for (int neighbour : neighbours)
        {
          answer = answer + grid.getPoint(neighbour) * values[neighbour];
          sum_responce += values[neighbour];
        }
        maxims.push_back(answer * (1.0 / sum_responce));      
      }
    }
    return maxims;
  }
};
