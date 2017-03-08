#include "stdafx.h"
#include <future>

using VectOfErrors = std::vector<double>;

// ----------------------------------------------------------------------------
// Returns a number in the range [a, b] and NOT in the range [a, b) as uniform_real_distibution does. See how nextafter() is used.
struct RndDbl {
public:
  explicit RndDbl(double const& low, double const& high) : mRandomEngine{ std::chrono::high_resolution_clock::now().time_since_epoch().count() }, mDistribution{ low, std::nextafter(high, DBL_MAX) } {
  }

  double operator()() {
    return mDistribution(mRandomEngine);
  }

private:
  std::mt19937_64                   mRandomEngine;
  std::uniform_real_distribution<>  mDistribution;
};

// ----------------------------------------------------------------------------
VectOfErrors ErrorsCalculation(double const& Radius) {                          // Each task works at radius constant (1 or 10 or 100...)
  
  auto         RadiusSquared = Radius * Radius;
  RndDbl       GenerateNumber { 0.0, Radius };                                  // GenerateNumber is a random generator of doubles between 0.0 and Radius
  VectOfErrors vErrors;                                                         // vErrors contains the differents errors while the # of points increases

  constexpr int NB_SET_OF_POINTS = 8;
  for (auto m = 0; m < NB_SET_OF_POINTS; ++m) {
    auto NbPoints         = pow(10, m);                                         // Nb of points in the form of 10^m where 0 <= m <= 7
    auto NbPointsInCircle = 0;

    for (auto n = 0; n < NbPoints; ++n) {
      auto x                   = GenerateNumber();
      auto y                   = GenerateNumber();
      auto RandomSquaredRadius = x*x + y*y;
      if (RandomSquaredRadius <= RadiusSquared) NbPointsInCircle++;
    }

    auto EstimatedPi    = (4.0 * NbPointsInCircle) / NbPoints;
    constexpr double PI = 3.14159265359;                                        // A reference value you can use for Pi is 3.14159265359
    vErrors.push_back(fabs(PI - EstimatedPi));
  }
  return vErrors;
}

// ----------------------------------------------------------------------------
int main() {
  
  std::vector<std::future<VectOfErrors>> ErrorsAtRadiusConstant;                // ErrorsAtRadiusConstant is a vector of futures. Each future is a vector of Error(s)
  
  constexpr int NB_RADIUS = 10;
  for (auto n = 0; n < NB_RADIUS; ++n) {
    auto Radius = pow(10, n);                                                   // Radius in the form of 10^n where 0 <= n <= 9 
    ErrorsAtRadiusConstant.push_back(std::async(std::launch::async, ErrorsCalculation, Radius));
  }

  std::cout << std::fixed << std::setprecision(9);                              // Modifies default formatting attributes for "nice" floating-point output
  for (auto &MyFuture : ErrorsAtRadiusConstant) {
    auto VectErr = MyFuture.get();                                              // Blocking. Wait for the each future
    for (auto &Err : VectErr) {                                                 // Print out the content of the future
      std::cout << Err << "\t";
    }
    std::cout << "\n";
  }

  std::cout << "Press ENTER to quit : ";
  std::cin.get();                                                               //std::cin.get() more expressive than std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}


/*
#include <chrono>
auto t0 = std::chrono::high_resolution_clock::now();                          // Benchmark purpose
auto t1 = std::chrono::high_resolution_clock::now();                          // Benchmark purpose
std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << " miliseconds\n";

*/