#include "day_17/day_17.h"

#include <day_17/tetris.h>
#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <cstdint>

namespace aoc
{
	static std::vector<Rock_move_direction> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::vector<Rock_move_direction> jets;
		for (char c = ' '; file >> c; )
		{
			switch (c)
			{
				case '<':
					jets.push_back(Rock_move_direction::LEFT);
					break;
				case '>':
					jets.push_back(Rock_move_direction::RIGHT);
					break;
				default:
					jets.push_back(Rock_move_direction::UNKNOWN);
					break;
			}
		}

		return jets;
	}

	std::string Day_17::part_1(const std::filesystem::path& input_path) const
	{
		const std::vector<Rock_move_direction> jets = parse_input(input_path / "day_17.txt");

		Tetris_simulator sim(jets);
		constexpr int64_t rocks_to_drop = 2022;
		for (int64_t i = 0; i < rocks_to_drop; ++i)
		{
			sim.drop_rock();
		}

		return fmt::format("Day 17 Part 1 | Rock tower height after {} rocks were dropped: {}", rocks_to_drop, sim.height());
	}

	std::string Day_17::part_2(const std::filesystem::path& input_path) const
	{
		const std::vector<Rock_move_direction> jets = parse_input(input_path / "day_17.txt");

		Tetris_simulator sim(jets);
		constexpr int64_t rocks_to_drop = 1000000000000;
		int64_t height = sim.drop_rocks_with_cycle_detection(rocks_to_drop);

		return fmt::format("Day 17 Part 2 | Rock tower height after {} rocks were dropped: {}", rocks_to_drop, height);
	}

} // aoc
