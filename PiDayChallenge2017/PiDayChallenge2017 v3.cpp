#include "stdafx.h"

#include <chrono>
#include <future>

// ----------------------------------------------------------------------------
// Returns a number in the range [a, b] and NOT in the range [a, b) as uniform_real_distibution does. See how nextafter() is used.
struct RndDouble {
public:
  explicit RndDouble(double const& low, double const& high) : mRandomEngine{ std::chrono::high_resolution_clock::now().time_since_epoch().count() }, mDistribution{ low, std::nextafter(high, DBL_MAX) } {
  }

  double operator()() {
    return mDistribution(mRandomEngine);
  }

private:
  std::mt19937_64                   mRandomEngine;
  std::uniform_real_distribution<>  mDistribution;
};

// ----------------------------------------------------------------------------
std::vector<double> MyTask(double const& Radius) {
  
  constexpr int M = 8; 
  std::vector<double> Distances(M);

  RndDouble GenerateNumber{ 0.0, Radius };                                      // Create a random number generator between 0 and Radius
  auto RadiusSquared = Radius * Radius;

  for (auto m = 0; m < M; ++m) {
    auto NbPoints = pow(10, m);                                                 // Nb of points in the form of 10^m where 0 <= m <= 7
    auto NbPointsIn = 0;

    for (auto j = 0; j < NbPoints; j++) {
      auto x = GenerateNumber();
      auto y = GenerateNumber();
      auto RandomSquaredRadius = x*x + y*y;
      if (RandomSquaredRadius <= RadiusSquared) NbPointsIn++;
    }

    auto             EstimatedPi = (4.0 * NbPointsIn) / NbPoints;
    constexpr double PI = 3.14159265359;                                        // A reference value you can use for Pi is 3.14159265359
    Distances[m] = fabs(PI - EstimatedPi);
  }
  return Distances;
}

// ----------------------------------------------------------------------------
int main() {
  
  auto time0 = std::chrono::high_resolution_clock::now();
  
  std::cout << std::fixed << std::setprecision(9);                              // Modifies the default formatting attribute for "nice" floating-point output
  
  std::vector<std::future<std::vector<double>>> Distances;                      // Distances is a vector to sote 
  
  for (auto n = 0; n < 10; ++n) {
    auto Radius = pow(10, n);                                                   // Radius in the form of 10^n where 0 <= n <= 9 
    Distances.push_back(std::async(std::launch::async, MyTask, Radius));
  }

  for (auto &d : Distances) {
    auto Results = d.get();
    for (auto &r : Results) {
      std::cout << r << "\t";
    }
    std::cout << std::endl;
  }
  
  auto time1 = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0).count() << " miliseconds passed\n";

  std::cout << "Press ENTER to quit : ";
  std::cin.get();                                                               //std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}
