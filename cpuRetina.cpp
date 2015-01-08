#include "cpuRetina.h"
#include <cmath>
int independent_execute(
    const std::vector<std::vector<uint8_t> > & input,
    std::vector<std::vector<uint8_t> > & output) {

  std::vector<const std::vector<uint8_t>* > converted_input;
  converted_input.resize(input.size());

  for (int i = 0; i < (int)input.size(); ++i) {
    converted_input[i] = &(input[i]);
  }

  std::cout << std::fixed << std::setprecision(2);
  logger::ll.verbosityLevel = 3;

  return cpuRetinaInvocation(converted_input, output);
}

void independent_post_execute(const std::vector<std::vector<uint8_t> > & output) {
    DEBUG << "post_execute invoked" << std::endl;
    DEBUG << "Size of output: " << output.size() << " B" << std::endl;
}

int cpuRetina(
    const std::vector<const std::vector<uint8_t>* > & input,
    std::vector<std::vector<uint8_t> > & output) {

  // Silent execution
  std::cout << std::fixed << std::setprecision(2);
  logger::ll.verbosityLevel = 3;
  return cpuRetinaInvocation(input, output);
}

struct Hit {
  double u,v;
};
struct Receptor
{
  double u, v;
  Receptor() {}
  Receptor(double _u, double _v) : u(_u), v(_v) {}
};

namespace {
  const int U_GRID_SIZE = 100;
  const double U_MIN = 0, U_MAX = 1;
  const int V_GRID_SIZE = 100;
  const double V_MIN = 0, V_MAX = 1;
  
  int getGridSize() 
  {
    return U_GRID_SIZE * V_GRID_SIZE;
  }
  
  int getIdFromIndexes(int u, int v)
  {
    return u + v * U_GRID_SIZE;
  }
  
  double getUFromId(int id)
  {
    int uId = id % U_GRID_SIZE;
    return uId * (U_MAX - U_MIN) / (U_GRID_SIZE - 1) + U_MIN;
  }
  
  double getVFromId(int id)
  {
    int vId = id / U_GRID_SIZE;
    return vId * (V_MAX - V_MIN) / (V_GRID_SIZE - 1) + V_MIN;
  }
   
  Receptor generateReceptorByNumber(int id)
  {
    return Receptor(getUFromId(id), getVFromId(id));
  }
  
}

std::vector<Receptor> generateGrid() {
  std::vector<Receptor> grid(getGridSize());
  for (int i = 0; i < (int)grid.size(); ++i) 
  {
    grid[i] = generateReceptorByNumber(i);
  }
  return grid;
}

double getDistanceFromReceptorToHit(Receptor receptor, Hit hit)
{
  return pow(receptor.u - receptor.u, 2) + pow(receptor.v - receptor.v, 2);
}
std::vector<double> cpuCalculateRetinaResponce(const std::vector<Receptor>& receptors, const std::vector<Hit>& hits, double sharpness)
{
  std::vector<double> responces;
  for (Receptor receptor : receptors) 
  {
    double sum = 0;
    for (Hit hit : hits)
    {
      sum += exp(-getDistanceFromReceptorToHit(receptor, hit) / sharpness);
    }
    responces.push_back(sum);
  }
  return responces;
}

std::vector<int> getNeighboursId(int uId, int vId)
{
  return std::vector<int> (
    {
     getIdFromIndexes(uId + 1, vId),
     getIdFromIndexes(uId - 1, vId), 
     getIdFromIndexes(uId, vId + 1), 
     getIdFromIndexes(uId, vId - 1)      
    }   
  );
}
/**
 * Common entrypoint for Gaudi and non-Gaudi
 * @param input  
 * @param output 
 */
int cpuRetinaInvocation(
    const std::vector<const std::vector<uint8_t>* > & input,
    std::vector<std::vector<uint8_t> > & output) 
{
  const std::vector<Hit> hits; //Get it some way from input
  const std::vector<Receptor> receptors = generateGrid();
  const std::vector<double> responce = cpuCalculateRetinaResponce(receptors, hits, 1.0);
  
  std::vector<Receptor> maximums;
  for (int uId = 1; uId < U_GRID_SIZE - 1; ++uId)
  {
    for (int vId = 1; vId < V_GRID_SIZE - 1; ++vId)
    {
      const std::vector<int> neighbourIds = getNeighboursId(uId, vId);
      const int currentId = getIdFromIndexes(uId, vId);
      double sumResponce = responce[currentId];
      bool isLocalMaximum = true;
      for (int neighbourId : neighbourIds) 
      {
        if (responce[currentId] < responce[neighbourId])
          isLocalMaximum = false;
        sumResponce += responce[neighbourId];
      }

      double uRestored = 0; 
      double vRestored = 0; 
      {
        double currentWeight = responce[currentId] / sumResponce;
        uRestored += getUFromId(currentId) * currentWeight;
        vRestored += getVFromId(currentId) * currentWeight;   	
      }
      if (isLocalMaximum)
	maximums.push_back(Receptor(uRestored, vRestored));
    }
  }
  
  //output maximus to some format;
  return 0;
}