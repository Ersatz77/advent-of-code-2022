#include "day_22/day_22.h"

#include "day_22/monkey_map.h"
#include "utility/io.h"
#include "utility/point.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

namespace aoc
{
	static std::pair<std::unordered_map<Point, char>, std::vector<Movement_instruction>> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::unordered_map<Point, char> grid;
		int y = 0;
		for (std::string line; std::getline(file, line) && !line.empty(); )
		{
			for (int x = 0; x < line.length(); ++x)
			{
				if (line[x] != ' ')
				{
					grid[Point(x + 1, y + 1)] = line[x];
				}
			}

			++y;
		}

		std::vector<Movement_instruction> instructions;
		while (file)
		{
			if (std::isdigit(file.peek()))
			{
				int amount = 0;
				file >> amount;

				instructions.emplace_back(Movement_type::MOVE, amount);
			}
			else
			{
				char turn = ' ';
				file >> turn;

				switch (turn)
				{
					case 'L':
						instructions.emplace_back(Movement_type::TURN_LEFT);
						break;
					case 'R':
						instructions.emplace_back(Movement_type::TURN_RIGHT);
						break;
					default:
						break;
				}
			}
		}

		return std::make_pair(grid, instructions);
	}

	std::string Day_22::part_1(const std::filesystem::path& input_path) const
	{
		const auto [grid, instructions] = parse_input(input_path / "day_22.txt");

		Monkey_map map(grid);
		for (const Movement_instruction& instruction : instructions)
		{
			map.walk(instruction);
		}

		return fmt::format("Day 22 Part 1 | Final password: {}", map.password());
	}

	std::string Day_22::part_2(const std::filesystem::path& input_path) const
	{
		const auto [grid, instructions] = parse_input(input_path / "day_22.txt");

		Monkey_map map(grid);
		for (const Movement_instruction& instruction : instructions)
		{
			map.walk(instruction, true);
		}

		return fmt::format("Day 22 Part 2 | Final password: {}", map.password());
	}

} // aoc
