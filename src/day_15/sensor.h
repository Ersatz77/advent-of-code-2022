#ifndef AOC_2022_DAY_15_SENSOR_H
#define AOC_2022_DAY_15_SENSOR_H

#include "utility/point.h"

namespace aoc
{
	struct Sensor
	{
		Sensor() = default;
		Sensor(const Point& sensor_pos, const Point& beacon_pos);

		bool has_within_range(const Point& other) const;
		bool has_unseen_points(const Point& min, const Point& max) const;

		Point pos;
		Point beacon_pos;
		int radius;
	};

} // aoc

#endif // !AOC_2022_DAY_15_SENSOR_H
