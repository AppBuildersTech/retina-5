/* 
 * File:   ClimbingHillOptimization.h
 * Author: towelenee
 *
 * Created on August 28, 2015, 9:44 PM
 */
#pragma once

template<class Arg>
class ClimbingHillOptimization : IOptimization<Arg>
{
public:
  ClimbingHillOptimization(
    const std::vector<Arg>& steps,
    double initialStepSize,
    const std::vector<Arg>& initialStates
  ) 
  {
    
  }
  
  std::vector<Arg> findMaximums(
    const std::function<double(Arg)> function
  )
  {
    
  }
private:
  std::vector<Arg> steps;
  double initialStepSize;
  std::vector<Arg> initialStates; 
};