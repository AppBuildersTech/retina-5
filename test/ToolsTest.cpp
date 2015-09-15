#define BOOST_TEST_MODULE ToolsTest
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "../src/Tools.h"
std::vector<std::pair<int, int> > setIntersectionValidate(
  std::vector<std::vector<int> >& eventsDx,
  std::vector<std::vector<int> >& eventsDy,
  int threshold
)
{
  std::vector<std::pair<int, int > > intersected;
  for (size_t x = 0; x < eventsDx.size(); ++x)
  {
    std::vector<int>& xHits = eventsDx[x];
    for (size_t y = 0; y < eventsDy.size(); ++y)
    {
      std::vector<int>& yHits = eventsDy[y];
      int intersection = 0;

      for (int i = 0; i < yHits.size(); i++)
      {
        for (int j = 0; j < xHits.size(); j++)
        {
          if (xHits[j] == yHits[i])
          {
            intersection++;
          }
        }
      }
      if (intersection >= threshold)
      {
        intersected.emplace_back(x, y);
      }
    }
  }
  return intersected;
}

BOOST_AUTO_TEST_CASE(intersection_test)
{
  const int sizeA = 123;
  const int sizeB = 234;
  const int size = 30;
  const int maxElement = 40;
  std::vector<std::vector<int> > a(sizeA);
  std::vector<std::vector<int> > b(sizeB);
  for (auto& vc: a)
  {
    vc = std::vector<int>(size);
    for (int &x : vc)
    {
      //todo: rewrite to random lib
      x = rand() % maxElement;
    }
  }

  for (auto& vc: b)
  {
    vc = std::vector<int>(size);
    for (int &x : vc)
    {
      //todo: rewrite to random lib
      x = rand() % maxElement;
    }
  }
  for (int i = 1; i <= size; i++)
  {
    std::vector<std::pair<int, int> > intersectValidate = setIntersectionValidate(a, b, i);
    std::vector<std::pair<int, int> > intersect = setIntersection(a, b, i);
    BOOST_CHECK_EQUAL(
      intersectValidate.size(), 
      intersect.size()
    );
    std::sort (intersect.begin(), intersect.end());
    std::sort (intersectValidate.begin(), intersectValidate.end());
    for (size_t i = 0; i < intersect.size(); ++i)
    {
      BOOST_CHECK_EQUAL(intersectValidate[i].first, intersect[i].first);
      BOOST_CHECK_EQUAL(intersectValidate[i].second, intersect[i].second);
    }
  }
}

