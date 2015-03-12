#pragma once
#include<vector>
#include<functional>

struct Dimension
{
  const double min;
  const double max;
  const int gridSize;
  Dimension(double min, double max, int gridSize) : min(min), max(max), gridSize(gridSize) {}
  
  double get(int index) const
  {
    if (index < 0 || index >= gridSize)
      return -1; //todo:throw exception
    return min + (max - min) / (gridSize - 1) * index;
  }
};

int calculateGridSize(const std::vector<Dimension>& dimensions, int noBoarders)
{
  int ans = 1;
  for (const Dimension& dim : dimensions)
    ans *= dim.gridSize - 2 * noBoarders;
  return ans;
}

void generateNextIndex(std::vector<int>& indexes, const std::vector<Dimension>& dim, int noBoarders)
{
  indexes[0]++;
  int i = 0;
  while (indexes[i] == dim[i].gridSize - noBoarders)
  {
    indexes[i] = noBoarders;
    ++indexes[i + 1];
    ++i;
  }
}

int multiIndexToIndex(const std::vector<int>& indexes, const std::vector<Dimension>& dim)
{
  int answer = indexes[indexes.size() - 1];
  for (int i = indexes.size() - 2; i >= 0; --i)
  {
    answer = answer * dim[i + 1].gridSize + indexes[i];
  }
  return answer;
}

template<class T>
std::vector<T> generateGrid(const std::vector<Dimension>& dimensions, std::function<T(const std::vector<int>&, const std::vector<Dimension>&)> generate)
{
  std::vector<int> indexes(dimensions.size(), 0);
  int gridSize = calculateGridSize(dimensions, 0);
  std::vector<T> ans(gridSize);
  for (int i = 0; i < gridSize; ++i)
  {
    ans[i] = generate(indexes, dimensions);
    generateNextIndex(indexes, dimensions, 0);
  }
  return ans;
}
