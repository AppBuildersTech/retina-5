#define BOOST_TEST_MODULE ToolsTest
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../src/optimizations/Grid.h"

BOOST_AUTO_TEST_CASE(gausian_generator_test)
{
  const int maxSize = 1000;
  for (int size = 3; size < maxSize; size += 2)
  {
    auto gauss = generateGaussDimension(size, 1, size);
    BOOST_CHECK_EQUAL(gauss.size(), size);
    BOOST_CHECK_EQUAL(gauss.size(), size);
    BOOST_CHECK_CLOSE(gauss[size / 2], size, 1e-2);
  }
}

