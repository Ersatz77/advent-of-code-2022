#include "day_23/day_23.h"

#include "utility/io.h"
#include "utility/point.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <cstdint>

namespace aoc
{
	static std::unordered_set<Point> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::unordered_set<Point> elves;
		size_t y = 0;
		for (std::string line; std::getline(file, line); )
		{
			for (size_t x = 0; x < line.length(); ++x)
			{
				if (line[x] == '#')
				{
					elves.insert(Point(x, y));
				}
			}

			++y;
		}

		return elves;
	}

	static std::array<std::array<Point, 3>, 4> get_deltas(const size_t index)
	{
		constexpr std::array<Point, 3> north = { { {0, -1}, {1, -1}, {-1, -1} } };
		constexpr std::array<Point, 3> south = { { {0, 1}, {1, 1}, {-1, 1} } };
		constexpr std::array<Point, 3> west = { { {-1, 0}, {-1, -1}, {-1, 1} } };
		constexpr std::array<Point, 3> east = { { {1, 0}, {1, -1}, {1, 1} } };

		constexpr std::array<std::array<std::array<Point, 3>, 4>, 4> deltas = { {
			{ north, south, west, east },
			{ south, west, east, north },
			{ west, east, north, south },
			{ east, north, south, west }
		} };

		return deltas[index];
	}

	static bool has_adjacent_elves(const std::unordered_set<Point>& elves, const Point& elf)
	{
		for (const Point& p : elf.adjacent())
		{
			if (elves.contains(p))
			{
				return true;
			}
		}

		return false;
	}

	static bool has_elves_in_direction(const std::unordered_set<Point>& elves, const Point& elf, const std::array<Point, 3>& deltas)
	{
		for (const Point& delta : deltas)
		{
			if (elves.contains(elf + delta))
			{
				return true;
			}
		}

		return false;
	}

	static int64_t step(std::unordered_set<Point>& elves, const std::array<std::array<Point, 3>, 4>& deltas)
	{
		std::unordered_map<Point, std::vector<Point>> movement_requests;
		for (const Point& elf : elves)
		{
			// Don't move if no elves are nearby
			if (!has_adjacent_elves(elves, elf))
			{
				movement_requests[elf].push_back(elf);
				continue;
			}

			// Elves were nearby, so we should try to find a new position
			Point requested_pos = elf;
			for (const auto& delta : deltas)
			{
				if (!has_elves_in_direction(elves, elf, delta))
				{
					requested_pos += delta.front();
					break;
				}
			}

			// Add elf to its new position request
			movement_requests[requested_pos].push_back(elf);
		}

		// Process movement requests
		std::unordered_set<Point> new_elves;
		int64_t movements = 0;
		for (const auto& [pos, requests] : movement_requests)
		{
			if (requests.size() == 1 && pos == requests.front())
			{
				// Elves that didn't move
				new_elves.insert(pos);
			}
			else if (requests.size() == 1)
			{
				// Elves that moved
				new_elves.insert(pos);
				++movements;
			}
			else
			{
				// Elves that tried moving to an already requested spot
				new_elves.insert(requests.begin(), requests.end());
			}
		}

		elves = new_elves;

		return movements;
	}

	std::string Day_23::part_1(const std::filesystem::path& input_path) const
	{
		std::unordered_set<Point> elves = parse_input(input_path / "day_23.txt");

		// Run simulation
		constexpr size_t rounds = 10;
		for (size_t round = 0; round < rounds; ++round)
		{
			step(elves, get_deltas(round % 4));
		}

		// Get rectangle bounds for area calculation
		int64_t x_min = std::numeric_limits<int64_t>::max();
		int64_t x_max = std::numeric_limits<int64_t>::min();
		int64_t y_min = std::numeric_limits<int64_t>::max();
		int64_t y_max = std::numeric_limits<int64_t>::min();

		for (const Point& elf : elves)
		{
			x_min = std::min(x_min, static_cast<int64_t>(elf.x));
			x_max = std::max(x_max, static_cast<int64_t>(elf.x));
			y_min = std::min(y_min, static_cast<int64_t>(elf.y));
			y_max = std::max(y_max, static_cast<int64_t>(elf.y));
		}

		// Subtract the area from the number of elves to get the number of empty spaces
		int64_t empty_spaces = (((x_max + 1) - x_min) * ((y_max + 1) - y_min)) - elves.size();

		return fmt::format("Day 23 Part 1 | Empty spaces after 10 rounds: {}", empty_spaces);
	}

	std::string Day_23::part_2(const std::filesystem::path& input_path) const
	{
		std::unordered_set<Point> elves = parse_input(input_path / "day_23.txt");

		int64_t round = 0;
		bool has_movement = true;
		while (has_movement)
		{
			if (step(elves, get_deltas(round % 4)) == 0)
			{
				has_movement = false;
			}

			++round;
		}

		return fmt::format("Day 23 Part 2 | Elves stop moving on round: {}", round);
	}

} // aoc
