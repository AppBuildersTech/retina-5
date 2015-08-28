#pragma once
#include<vector>
#include<functional>


std::vector<double> generateUniformDimension(double min, double max, size_t size);

std::vector<double> generateGaussDimension(double mean, double sd, size_t size);

template<class T>
class Grid
{
public:
  Grid(
    const std::vector<std::vector<double> >& dimensions,
    const std::function<T(std::vector<double>)>& generator
  ) : dimensions(dimensions), generator(generator)
  {
    size_t mul = 1;
    for (const std::vector<double>& dimension: dimensions)
    {
      mul *= dimension.size();
    }
    size_ = mul;
  }
  size_t size() const
  {
    return size_;
  }
  
  size_t dim() const 
  {
    return dimensions.size();
  }
  
  std::vector<int> getNeighbours(int index) const 
  {
    std::vector<int> neighbours;
    std::vector<int> multiIndex = indexToMultiIndex(index);
    std::vector<int> multiIndexCopy = multiIndex;
    for (int i = 0; i < multiIndex.size(); ++i)
    {
      for (int dx : {-1, 1})
      {
        multiIndexCopy[i] = multiIndex[i] + dx;
        if (multiIndexCopy[i] >= 0 && multiIndex[i] < dimensions[i].size())
        {
          neighbours.push_back(multiIndexToIndex(multiIndexCopy));
        }
      }
      multiIndexCopy[i] = multiIndex[i];
    }
    return neighbours;
  }
  
  T getPoint(int index) const
  {
    std::vector<int> multiIndex = indexToMultiIndex(index);
    std::vector<double> params(dimensions.size());
    for (size_t i = 0; i < dimensions.size(); i++)
    {
      params[i] = dimensions[i][multiIndex[i]];
    }
    return generator(params);
  }
 
private:
  const std::vector<std::vector<double> > dimensions;
  const std::function<T(std::vector<double>)> generator;
  size_t size_;
  int multiIndexToIndex(
    const std::vector<int>& multiIndex 
  ) const 
  {
    int multiplier = 1;
    int index = 0;
    for (size_t i = 0; i < dimensions.size(); ++i)
    {
      index += multiIndex[i] * multiplier;
      multiplier *= dimensions[i].size();
    }
    return index;
  }

  std::vector<int> indexToMultiIndex(int index) const
  {
    std::vector<int> multiIndex(dimensions.size());
    for (size_t i = 0; i < dimensions.size(); ++i)
    {
      multiIndex[i] = index % dimensions[i].size();
      index /= dimensions[i].size();
    }
    return std::move(multiIndex);
  }

};