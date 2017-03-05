#include "stdafx.h"
#include <algorithm>
#include <numeric>

using Point = std::pair<double, double>;

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
std::vector<Point> GeneratePointsInSquare(double const & r, int const &n) {
	
	RndDouble GenerateNumber{ 0.0, r };

	std::vector<Point> Points(n);
	Points.reserve(n);

	std::for_each(Points.begin(), Points.end(), [&GenerateNumber](Point &p) {
		p.first  = GenerateNumber();
		p.second = GenerateNumber();
	});
	return Points;
}

// ----------------------------------------------------------------------------
int CountPointsInCircle(std::vector<Point> Points, double const r) {
	
	int count      = 0;
	double rsquare = r*r;

	std::for_each(Points.begin(), Points.end(), [&rsquare, &count](Point &p) {
		if ((p.first * p.first + p.second * p.second) < rsquare) count++;
	});
	return count;
}

// ----------------------------------------------------------------------------
double CalculateDistance(int const & NbPoints, int const & NbPointsIn) {

	constexpr double PI = 3.14159265359;

	auto EstimatedPi = (4.0 * NbPointsIn) / NbPoints;
	return fabs(PI - EstimatedPi);
}

// ----------------------------------------------------------------------------
int main() {

	std::cout << std::fixed << std::setprecision(9);
	
	std::vector<double>Radius(10);  
	std::iota(Radius.begin(), Radius.end(), 0);
	std::transform(Radius.begin(), Radius.end(), Radius.begin(), [](double const& n) { return pow(10, n); });
	
	std::vector<int>NbPoints(7);		//8 
	std::iota(NbPoints.begin(), NbPoints.end(), 0);
	std::transform(NbPoints.begin(), NbPoints.end(), NbPoints.begin(), [](int const& m) { return static_cast<int> (pow(10, m) ); });
	
	for (auto const& r : Radius) {
		for (auto const& n : NbPoints) {
			auto AllPoints  = GeneratePointsInSquare(r, n);
			auto NbPointsIn = CountPointsInCircle(AllPoints, r);
			auto Distance   = CalculateDistance(n, NbPointsIn);
			std::cout << Distance << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "Press ENTER to quit : ";
	std::cin.get();
}