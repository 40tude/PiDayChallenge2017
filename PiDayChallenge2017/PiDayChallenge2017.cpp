#include "stdafx.h"
#include <iomanip>

// ----------------------------------------------------------------------------
// Returns a number in the range [a, b] and NOT in the range [a, b) as uniform_real_distibution does. See how nextafter() is used.
struct Rand_Double {
public:
																																								
	Rand_Double(double low, double high) : mRandomEngine { std::chrono::high_resolution_clock::now().time_since_epoch().count() }, mDistribution { low, std::nextafter(high, DBL_MAX) } {}

	double operator()() {
		return mDistribution(mRandomEngine);
	}

private:
	std::mt19937_64 									mRandomEngine;
	std::uniform_real_distribution<>	mDistribution;
};

// ----------------------------------------------------------------------------
int main() {
	std::cout << std::fixed;
	std::cout << std::setprecision(9);
	
	for (auto n = 0; n <10; ++n) {
		auto Radius					= pow(10, n);																						// Radius in the form of 10^n where 0 <= n <= 9 
		auto RadiusSquared	= Radius * Radius;

		Rand_Double GenerateNumber {0.0, Radius};																		// Create a random number generator between 0 and CurrentRadius

		for (auto m = 0; m < 7; ++m) {																							
			auto NbPointsIn = 0;
			int NbPoints		= static_cast<int>(pow(10, m));														// Nb of points in the form of 10^m where 0 <= m <= 7

			for (auto j = 0; j < NbPoints; j++) {
				auto x										= GenerateNumber();
				auto y										= GenerateNumber();
				auto RandomSquaredRadius	= x*x + y*y;
				if (RandomSquaredRadius <= RadiusSquared) NbPointsIn++;
			}
			
			auto EstimatedPi = (4.0 * NbPointsIn) / NbPoints;
			constexpr double PI = 3.14159265359;																			// A reference value you can use for Pi is 3.14159265359
			std::cout << std::fabs(PI - EstimatedPi) << "\t";												// The output should show the distance between the estimates and a reference value of Pi. 
			//std::cout << std::setw(10) << EstimatedPi << "\t";
			//std::cout << EstimatedPi << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "Press ENTER to quit : ";
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}