#include "utility/math.h"

#include "utility/point.h"

#include <cmath>

namespace aoc
{
	double manhattan_distance(const Point& p1, const Point& p2)
	{
		return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
	}

	double manhattan_distance(const Point_3d& p1, const Point_3d& p2)
	{
		return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y) + std::abs(p1.z - p2.z);
	}

} // aoc
