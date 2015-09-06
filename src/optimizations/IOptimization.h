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
  public:
  virtual std::vector<Arg> findMaximums(const std::function<double(Arg)> function) const = 0;
  virtual std::vector<Arg> findMaximums(
    const std::function<std::vector<double>(const std::vector<Arg>&)> function
  ) const = 0;

};