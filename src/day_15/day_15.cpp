#include "day_15/day_15.h"

#include "day_15/sensor.h"
#include "utility/io.h"
#include "utility/point.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <vector>
#include <array>
#include <stack>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cstdint>
#include <utility>


namespace aoc
{
	static std::vector<Sensor> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::vector<Sensor> sensors;
		for (std::string line; std::getline(file, line) && !line.empty(); )
		{
			std::istringstream line_stream(line);
			Point sensor_pos;
			Point beacon_pos;

			line_stream.ignore(12);
			line_stream >> sensor_pos.x;

			line_stream.ignore(4);
			line_stream >> sensor_pos.y;

			line_stream.ignore(25);
			line_stream >> beacon_pos.x;

			line_stream.ignore(4);
			line_stream >> beacon_pos.y;

			sensors.emplace_back(sensor_pos, beacon_pos);
		}

		return sensors;
	}

	static Point find_distress_signal(const std::vector<Sensor>& sensors, const Point& min_pos, const Point& max_pos)
	{
		std::stack<std::pair<Point, Point>> quadrants_to_process({ { min_pos, max_pos } });
		while (!quadrants_to_process.empty())
		{
			// Get the quadrant to search
			const auto [min, max] = quadrants_to_process.top();
			quadrants_to_process.pop();

			// Return position if the quadrant is a single point
			if (min == max && std::none_of(sensors.begin(), sensors.end(),
				[&min](const Sensor& s) { return s.has_within_range(min); }))
			{
				return min;
			}
			
			// Break this quadrant into 4 smaller quadrants
			const Point mid(std::floor((min.x + max.x) / 2), std::floor((min.y + max.y) / 2));
			const std::array<std::pair<Point, Point>, 4> quadrants = { {
				{ min, mid },
				{ { mid.x + 1, min.y }, { max.x, mid.y } },
				{ { min.x, mid.y + 1 }, { mid.x, max.y } },
				{ { mid.x + 1, mid.y + 1 }, max }
			} };

			// Check if quadrants have unseen points
			for (const auto& [quad_min, quad_max] : quadrants)
			{
				// Skip this quadrant if its min and max points are out of order
				if (quad_min.x > quad_max.x || quad_min.y > quad_max.y)
					continue;

				// If all sensors have unseen corner points, add this quadrant
				if (std::all_of(sensors.begin(), sensors.end(),
					[&quad_min, &quad_max](const Sensor& s) { return s.has_unseen_points(quad_min, quad_max); }))
				{
					quadrants_to_process.emplace(quad_min, quad_max);
				}
			}
		}

		return Point::origin;
	}

	std::string Day_15::part_1(const std::filesystem::path& input_path) const
	{
		const std::vector<Sensor> sensors = parse_input(input_path / "day_15.txt");

		int min_x = std::numeric_limits<int>::max();
		int max_x = std::numeric_limits<int>::min();
		int max_range = std::numeric_limits<int>::min();

		for (const Sensor& sensor : sensors)
		{
			min_x = std::min(min_x, static_cast<int>(sensor.beacon_pos.x));
			max_x = std::max(max_x, static_cast<int>(sensor.beacon_pos.x));
			max_range = std::max(max_range, sensor.radius);
		}

		constexpr int row = 2000000;
		int points_without_beacons = 0;
		for (int x = min_x - max_range; x <= max_x + max_range; ++x)
		{
			// Check if this point is in range of a sensor
			Point current_pos(x, row);
			for (const Sensor& sensor : sensors)
			{
				// Break out of the loop if this point is occupied by a sensor or beacon
				if (sensor.pos == current_pos || sensor.beacon_pos == current_pos)
					break;

				// Check if this sensor has this point in range of its beacon
				if (sensor.has_within_range(current_pos))
				{
					++points_without_beacons;
					break;
				}
			}
		}

		return fmt::format("Day 15 Part 1 | Positions without a beacon on row {}: {}", row, points_without_beacons);
	}

	std::string Day_15::part_2(const std::filesystem::path& input_path) const
	{
		const std::vector<Sensor> sensors = parse_input(input_path / "day_15.txt");

		const Point min_pos(0, 0);
		const Point max_pos(4000000, 4000000);

		Point distress_signal = find_distress_signal(sensors, min_pos, max_pos);
		int64_t tuning_frequency = (distress_signal.x * max_pos.x) + distress_signal.y;

		return fmt::format("Day 15 Part 2 | Tuning frequency: {}", tuning_frequency);
	}

} // aoc
