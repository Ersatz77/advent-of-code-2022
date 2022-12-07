#include "day_5/day_5.h"

#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <stack>
#include <utility>
#include <algorithm>
#include <cctype>

namespace aoc
{
	struct Stacking_step
	{
		int move;
		size_t from_idx;
		size_t to_idx;
	};

	static std::pair<std::vector<std::stack<char>>, std::vector<Stacking_step>> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		// Create stacks
		std::vector<std::string> lines;
		for (std::string line; std::getline(file, line) && !line.empty(); )
		{
			lines.emplace_back(line);
		}

		std::reverse(lines.begin(), lines.end());

		std::vector<std::stack<char>> stacks(9);
		for (size_t i = 1; i < lines.size(); ++i)
		{
			size_t stack = 0;
			for (size_t j = 1; j < lines[i].size(); j += 4)
			{
				const char c = lines[i][j];
				if (!isspace(c))
				{
					stacks[stack].push(c);
				}
				++stack;
			}
		}

		// Create steps
		std::vector<Stacking_step> steps;
		int move = 0;
		size_t from_idx = 0;
		size_t to_idx = 0;
		std::string junk;
		while (file >> junk >> move >> junk >> from_idx >> junk >> to_idx)
		{
			steps.emplace_back(move, from_idx - 1, to_idx - 1);
		}

		return std::make_pair(stacks, steps);
	}

	static std::string get_top_crates(const std::vector<std::stack<char>>& stacks)
	{
		std::string top_chars;
		for (const auto& stack : stacks)
		{
			if (!stack.empty())
			{
				top_chars += stack.top();
			}
		}

		return top_chars;
	}

	std::string Day_5::part_1(const std::filesystem::path& input_path) const
	{
		auto [stacks, steps] = parse_input(input_path / "day_5.txt");

		// Perform stacking steps
		for (const Stacking_step& step : steps)
		{
			for (size_t i = 0; i < step.move; ++i)
			{
				char c = stacks[step.from_idx].top();
				stacks[step.from_idx].pop();
				stacks[step.to_idx].push(c);
			}
		}

		return fmt::format("Day 5 Part 1 | Crates at the top of the stacks: {}", get_top_crates(stacks));
	}

	std::string Day_5::part_2(const std::filesystem::path& input_path) const
	{
		auto [stacks, steps] = parse_input(input_path / "day_5.txt");

		// Perform stacking steps
		for (const Stacking_step& step : steps)
		{
			std::string to_move;
			for (size_t i = 0; i < step.move; ++i)
			{
				char c = stacks[step.from_idx].top();
				stacks[step.from_idx].pop();
				to_move += c;
			}

			for (size_t i = to_move.length(); i > 0; --i)
			{
				stacks[step.to_idx].push(to_move[i - 1]);
			}
		}

		return fmt::format("Day 5 Part 2 | Crates at the top of the stacks: {}", get_top_crates(stacks));
	}

} // aoc
