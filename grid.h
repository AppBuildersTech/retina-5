#pragma once
#include<vector>
#include<functional>

struct Dimension
{
  const double min;
  const double max;
  const int gridSize;
  Dimension(double min, double max, int gridSize) : min(min), max(max), gridSize(gridSize) {}
  
  double getGridBoarder(int index) const;
};

int calculateGridSize(const std::vector<Dimension>& dimensions, const int noBoarders);

void generateNextMultiIndex(std::vector<int>& indexes, const std::vector<Dimension>& dim, int noBoarders);

int multiIndexToIndex(const std::vector<int>& indexes, const std::vector<Dimension>& dim);

std::vector<int> generateNeighboursIndexes(const std::vector<int>& indexes, const std::vector<Dimension>& dim);

template<class T>
std::vector<T> generateGrid(
    const std::vector<Dimension>& dimensions, 
    std::function<T(const std::vector<int>&, const std::vector<Dimension>&)> generateOne
)
{
  std::vector<int> multiIndex(dimensions.size(), 0);
  int gridSize = calculateGridSize(dimensions, 0);
  std::vector<T> ans(gridSize);
  for (int i = 0; i < gridSize; ++i)
  {
    ans[i] = generateOne(multiIndex, dimensions);
    generateNextMultiIndex(multiIndex, dimensions, 0);
  }
  return ans;
}
