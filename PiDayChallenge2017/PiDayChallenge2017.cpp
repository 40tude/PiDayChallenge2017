#include "stdafx.h"

// ----------------------------------------------------------------------------
// Returns a number in the range [a, b] and NOT in the range [a, b) as uniform_real_distibution does. See how nextafter() is used.
struct RndDouble {
public:
	explicit RndDouble(double const& low, double const& high) : mRandomEngine{ std::random_device()() }, mDistribution{ low, std::nextafter(high, DBL_MAX) } {
	}

  double operator()() {
    return mDistribution(mRandomEngine);
  }

private:
  std::mt19937_64                   mRandomEngine;
  std::uniform_real_distribution<>  mDistribution;
};

// ----------------------------------------------------------------------------
int main() {
  std::cout << std::fixed << std::setprecision(9);                              // Modifies the default formatting for floating-point output
  
  for (auto n = 0; n <10; ++n) {
    auto Radius         = pow(10, n);                                           // Radius in the form of 10^n where 0 <= n <= 9 
    auto SquaredRadius  = Radius * Radius;

    RndDouble GenerateRndDbl {0.0, Radius};                                     // Create generator of random double in the range of [0, Radius]

    for (auto m = 0; m < 8; ++m) {
      auto TotalNbPoints    = pow(10, m);                                       // Nb of points in the form of 10^m where 0 <= m <= 7
      auto NbPointsInCircle = 0;

      for (auto j = 0; j < TotalNbPoints; j++) {
        auto x                    = GenerateRndDbl();
        auto y                    = GenerateRndDbl();
        auto RandomSquaredRadius  = x*x + y*y;
        if (RandomSquaredRadius <= SquaredRadius) NbPointsInCircle++;
      }

      auto EstimatedPi    = (4.0 * NbPointsInCircle) / TotalNbPoints;
      constexpr double PI = 3.14159265359;                                      // A reference value you can use for Pi is 3.14159265359
      std::cout << std::fabs(PI - EstimatedPi) << "\t";                         // The output should show the distance between the estimates and a reference value of Pi. 
    }
    std::cout << std::endl;
  }
  std::cout << "Press ENTER to quit : ";
  std::cin.get();                                                               //std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}
