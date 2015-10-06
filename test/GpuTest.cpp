#define BOOST_TEST_MODULE GpuTest
#include <boost/test/unit_test.hpp>

#include <chrono>
#include "../src/RetinaCore/GpuRetina.cuh"
#include "../src/RetinaCore/CpuRetina.h"
#include <cstdio>

#include <random>

std::vector<Hit> generateRandomHits(int N)
{
  std::default_random_engine generator;
  std::normal_distribution<double> distribution(5.0,2.0);
  
  std::vector<Hit> generated(N);
  for (Hit& hit : generated)
  {
    hit.x = distribution(generator);
    hit.y = distribution(generator);
    hit.z = distribution(generator);
  }
  return std::move(generated);
}

std::vector<TrackProjection> generateRandomTrackProjections(int N)
{
  std::default_random_engine generator;
  std::normal_distribution<double> distribution(5.0,2.0);
  
  std::vector<TrackProjection> generated(N);
  for (TrackProjection& track : generated)
  {
    track.x0 = distribution(generator);
    track.dx = distribution(generator);
  }
  return std::move(generated);
}

std::vector<TrackPure> generateRandomTrackPure(int N)
{
  std::default_random_engine generator;
  std::normal_distribution<double> distribution(5.0,2.0);
  
  std::vector<TrackPure> generated(N);
  for (TrackPure& track : generated)
  {
    track.x0 = distribution(generator);
    track.dx = distribution(generator);
    track.y0 = distribution(generator);
    track.dy = distribution(generator);
  }
  return std::move(generated);
}

BOOST_AUTO_TEST_CASE( retina_test_3d )
{
  const int H = 2560;
  for (int N = 1 << 6; N <= (1 << 13); N <<= 1)
  {
    std::vector<Hit> hits = generateRandomHits(H);
    std::vector<TrackPure> tracks = generateRandomTrackPure(N);
    std::vector<double> valueCpu(tracks.size());
    std::vector<double> valueGpu(tracks.size());

    std::cout << N << "\t";
    {
      auto timeStart = std::chrono::high_resolution_clock::now();

      getRetina3dCpu(
        tracks.data(),
        tracks.size(),
        hits.data(),
        hits.size(),
        1e3,
        valueCpu.data()
      );
      auto timeEnd = std::chrono::high_resolution_clock::now();

      printf("%lf\t", std::chrono::duration<double, std::milli>(timeEnd-timeStart).count());
    }

    {
      auto timeStart = std::chrono::high_resolution_clock::now();

      getRetina3dGpu(
        tracks.data(),
        tracks.size(),
        hits.data(),
        hits.size(),
        1e3,
        valueGpu.data()
      );    
      auto timeEnd = std::chrono::high_resolution_clock::now();

      printf("%lf\n", std::chrono::duration<double, std::milli>(timeEnd-timeStart).count());
    }
    for (int trackId = 0; trackId < tracks.size(); trackId++)
    {
      BOOST_CHECK_CLOSE(valueCpu[trackId], valueGpu[trackId], 1e-6);
    }
  }
}

BOOST_AUTO_TEST_CASE( retina_test_2d )
{
  const int H = 2560;
  for (int N = 1 << 6; N <= (1 << 13); N <<= 1)
  {
    std::vector<Hit> hits = generateRandomHits(H);
    std::vector<TrackProjection> tracks = generateRandomTrackProjections(N);
    
    std::vector<double> valueCpu(tracks.size());
    std::vector<double> valueGpu(tracks.size());

    std::vector<double> hitsX(hits.size());
    std::vector<double> hitsZ(hits.size());

    std::cout << N << "\t";
    {
      auto timeStart = std::chrono::high_resolution_clock::now();
      getRetina2dCpu(
        tracks.data(),
        tracks.size(),
        hitsX.data(),
        hitsZ.data(),
        hits.size(),
        1e3,
        valueCpu.data()
      );    
      auto timeEnd = std::chrono::high_resolution_clock::now();

      printf("%lf\t", std::chrono::duration<double, std::milli>(timeEnd-timeStart).count());
    }
    {
      auto timeStart = std::chrono::high_resolution_clock::now();
      getRetina2dGpu(
        tracks.data(),
        tracks.size(),
        hitsX.data(),
        hitsZ.data(),
        hits.size(),
        1e3,
        valueGpu.data()
      );
      auto timeEnd = std::chrono::high_resolution_clock::now();

      printf("%lf\n", std::chrono::duration<double, std::milli>(timeEnd-timeStart).count());
    }
    for (int trackId = 0; trackId < tracks.size(); trackId++)
    {
      BOOST_CHECK_CLOSE(valueCpu[trackId], valueGpu[trackId], 1e-6);
    }
  }
}

