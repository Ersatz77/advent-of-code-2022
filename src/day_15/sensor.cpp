#include "day_15/sensor.h"

#include "utility/point.h"
#include "utility/math.h"

#include <array>
#include <algorithm>
#include <limits>

namespace aoc
{
	Sensor::Sensor(const Point& sensor_pos, const Point& beacon_pos) :
		pos(sensor_pos), beacon_pos(beacon_pos), radius(0)
	{
		radius = static_cast<int>(manhattan_distance(pos, beacon_pos));
	}

	bool Sensor::has_within_range(const Point& other) const
	{
		int distance = static_cast<int>(manhattan_distance(pos, other));
		return distance <= radius;
	}

	bool Sensor::has_unseen_points(const Point& min, const Point& max) const
	{
		int largest_distance = std::numeric_limits<int>::min();
		const std::array<Point, 4> corners = { { min, { max.x, min.y }, { min.x, max.y }, max } };
		for (const Point& corner : corners)
		{
			largest_distance = std::max(largest_distance, static_cast<int>(manhattan_distance(corner, pos)));
		}

		return largest_distance > radius;
	}

} // aoc
