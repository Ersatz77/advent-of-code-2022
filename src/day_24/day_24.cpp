#include "day_24/day_24.h"

#include "day_24/valley.h"
#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace aoc
{
	static Valley parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		// Create blizzards
		std::vector<Blizzard> blizzards;
		size_t y = 0;
		for (std::string line; std::getline(file, line); )
		{
			for (size_t x = 0; x < line.length(); ++x)
			{
				switch (line[x])
				{
					case '^':
						blizzards.emplace_back(Point(x, y), Blizzard_direction::NORTH);
						break;
					case 'v':
						blizzards.emplace_back(Point(x, y), Blizzard_direction::SOUTH);
						break;
					case '>':
						blizzards.emplace_back(Point(x, y), Blizzard_direction::EAST);
						break;
					case '<':
						blizzards.emplace_back(Point(x, y), Blizzard_direction::WEST);
						break;
					default:
						break;
				}
			}

			++y;
		}

		return Valley(blizzards);
	}

	std::string Day_24::part_1(const std::filesystem::path& input_path) const
	{
		Valley valley = parse_input(input_path / "day_24.txt");
		Point start = valley.start();
		Point end = valley.end();

		return fmt::format("Day 24 Part 1 | Minimum amount of time required to cross the valley: {}", valley.shortest_time_from(start, end));
	}

	std::string Day_24::part_2(const std::filesystem::path& input_path) const
	{
		Valley valley = parse_input(input_path / "day_24.txt");
		Point start = valley.start();
		Point end = valley.end();

		int64_t minutes = 0;
		for (int i = 0; i < 3; ++i)
		{
			minutes += valley.shortest_time_from(start, end);
			std::swap(start, end);
		}

		return fmt::format("Day 24 Part 2 | Minimum amount of time required to cross the valley 3 times: {}", minutes);
	}

} // aoc
