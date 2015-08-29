/* 
 * File:   IOptimization.h
 * Author: towelenee
 *
 * Created on August 28, 2015, 9:26 PM
 */

#pragma once
template<class Arg>
class IOptimization
{
  virtual std::vector<Arg> findMaximums(const std::function<double(Arg)> function) = 0;
};