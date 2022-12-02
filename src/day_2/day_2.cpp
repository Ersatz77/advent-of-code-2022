#include "day_2/day_2.h"

#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <utility>

namespace aoc
{
	static std::vector<std::pair<int, int>> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = aoc::open_file(path);

		std::vector<std::pair<int, int>> input;
		char opponent_move_char = ' ';
		char your_move_char = ' ';
		while (file >> opponent_move_char >> your_move_char)
		{
			int opponent_move = (opponent_move_char > 'C' ? opponent_move_char - 'X' : opponent_move_char - 'A');
			int your_move = (your_move_char > 'C' ? your_move_char - 'X' : your_move_char - 'A');
			input.emplace_back(opponent_move, your_move);
		}

		return input;
	}

	std::string Day_2::part_1(const std::filesystem::path& input_path) const
	{
		std::vector<std::pair<int, int>> input = parse_input(input_path / "day_2.txt");

		int total_score = 0;
		for (const auto& game : input)
		{
			int opponent = game.first;
			int you = game.second;
			if (opponent == ((you + 2) % 3))
			{
				// Win
				total_score += you + 1 + 6;
			}
			else if (opponent == you)
			{
				// Draw
				total_score += you + 1 + 3;
			}
			else
			{
				// Lose
				total_score += you + 1;
			}
		}

		return fmt::format("Day 2 Part 1 | Rock paper scissors score: {}", total_score);
	}

	std::string Day_2::part_2(const std::filesystem::path& input_path) const
	{
		std::vector<std::pair<int, int>> input = parse_input(input_path / "day_2.txt");

		int total_score = 0;
		for (const auto& game : input)
		{
			int opponent = game.first;
			int result = game.second;
			switch (result)
			{
				case 0:
					// Lose
					total_score += ((opponent + 2) % 3) + 1;
					break;
				case 1:
					// Draw
					total_score += opponent + 1 + 3;
					break;
				case 2:
					// Win
					total_score += ((opponent + 1) % 3) + 1 + 6;
					break;
			}
		}

		return fmt::format("Day 2 Part 2 | Rock paper scissors score: {}", total_score);
	}

} // aoc
