#include "day_14/day_14.h"

#include "utility/io.h"
#include "utility/string.h"
#include "utility/point.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <algorithm>

namespace aoc
{
	static Point parse_point(const std::string& str)
	{
		std::istringstream point_stream(str);
		double x = 0.0;
		double y = 0.0;
		
		point_stream >> x;
		point_stream.ignore(1);
		point_stream >> y;

		return { x, y };
	}

	static void draw_line(std::unordered_set<Point>& cave, const Point& begin, const Point& end)
	{
		double min_x = std::min(begin.x, end.x);
		double max_x = std::max(begin.x, end.x);

		double min_y = std::min(begin.y, end.y);
		double max_y = std::max(begin.y, end.y);

		for (double x = min_x; x <= max_x; ++x)
		{
			for (double y = min_y; y <= max_y; ++y)
			{
				cave.emplace(x, y);
			}
		}
	}

	static std::unordered_set<Point> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::unordered_set<Point> cave;
		for (std::string line; std::getline(file, line); )
		{
			std::vector<std::string> line_parts = regex_split(line, " -> ");
			for (size_t i = 1; i < line_parts.size(); ++i)
			{
				Point begin = parse_point(line_parts[i - 1]);
				Point end = parse_point(line_parts[i]);
				draw_line(cave, begin, end);
			}
		}

		return cave;
	}

	static bool drop_sand(std::unordered_set<Point>& cave, const double max_y, const Point& drop_at, const bool part_2 = false)
	{
		Point current = drop_at;
		while (true)
		{
			// Break out of loop if we're falling into the abyss
			if (current.y >= max_y)
				break;

			// Try to move sand to a new position
			bool moved = false;
			constexpr std::array<Point, 3> deltas = { { {0.0, 1.0}, { -1.0, 1.0 }, { 1.0, 1.0 }} };
			for (const Point& delta : deltas)
			{
				Point next = current + delta;
				if (!cave.contains(next) && (part_2 ? next.y < max_y : true))
				{
					current = next;
					moved = true;
					break;
				}
			}

			// If we didn't move, add sand to the cave and return
			if (!moved)
			{
				cave.insert(current);
				return true;
			}
		}

		// If we got this far, the sand stopped moving
		return false;
	}

	std::string Day_14::part_1(const std::filesystem::path& input_path) const
	{
		std::unordered_set<Point> cave = parse_input(input_path / "day_14.txt");

		double max_y = 0.0;
		for (const Point& p : cave)
		{
			max_y = std::max(max_y, p.y);
		}

		Point emitter{ 500, 0 };
		int sand = 0;
		while (drop_sand(cave, max_y, emitter))
		{
			++sand;
		}
		
		return fmt::format("Day 14 Part 1 | Sand that came to rest before falling into the abyss: {}", sand);
	}

	std::string Day_14::part_2(const std::filesystem::path& input_path) const
	{
		std::unordered_set<Point> cave = parse_input(input_path / "day_14.txt");

		double max_y = 0.0;
		for (const Point& p : cave)
		{
			max_y = std::max(max_y, p.y + 2);
		}

		Point emitter{ 500, 0 };
		int sand = 0;
		while (drop_sand(cave, max_y, emitter, true))
		{
			++sand;

			if (cave.contains(emitter))
				break;
		}

		return fmt::format("Day 14 Part 2 | Sand that came to rest before blocking the emitter: {}", sand);
	}

} // aoc
