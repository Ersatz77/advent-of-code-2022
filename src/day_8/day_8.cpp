#include "day_8/day_8.h"

#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <cstdint>
#include <utility>

namespace aoc
{
	static std::vector<std::vector<int>> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::vector<std::vector<int>> grid;
		for (std::string line; std::getline(file, line); )
		{
			std::vector<int> row;
			for (size_t i = 0; i < line.length(); ++i)
			{
				row.push_back(std::stoi(line.substr(i, 1)));
			}

			grid.push_back(row);
		}

		return grid;
	}

	static std::pair<bool, size_t> check_tree(const std::vector<std::vector<int>>& grid, size_t x, size_t y, const size_t dx, const size_t dy)
	{
		int starting_height = grid[y][x];
		x += dx;
		y += dy;

		bool is_visible = false;
		uint64_t score = 1;
		while (true)
		{
			// Check if the current height is the same height or taller than the starting height
			if (grid[y][x] >= starting_height)
			{
				break;
			}

			// If we've reached the border, the tree is visible
			if (x == 0 || x == grid.front().size() - 1 || y == 0 || y == grid.size() - 1)
			{
				is_visible = true;
				break;
			}

			x += dx;
			y += dy;
			++score;
		}

		return std::make_pair(is_visible, score);
	}

	std::string Day_8::part_1(const std::filesystem::path& input_path) const
	{
		std::vector<std::vector<int>> grid = parse_input(input_path / "day_8.txt");

		const size_t width = grid.front().size();
		const size_t height = grid.size();

		uint64_t visible_trees = (width * 2) + ((height - 2) * 2);
		for (size_t y = 1; y < height - 1; ++y)
		{
			for (size_t x = 1; x < width - 1; ++x)
			{
				constexpr std::array<std::array<int, 2>, 4> deltas = { {{1, 0}, {-1, 0}, {0, 1}, {0, -1}} };
				for (const auto& d : deltas)
				{
					const auto [is_visible, _] = check_tree(grid, x, y, d[0], d[1]);
					if (is_visible)
					{
						++visible_trees;
						break;
					}
				}

			}
		}

		return fmt::format("Day 8 Part 1 | Total visible trees: {}", visible_trees);
	}

	std::string Day_8::part_2(const std::filesystem::path& input_path) const
	{
		std::vector<std::vector<int>> grid = parse_input(input_path / "day_8.txt");

		const size_t width = grid.front().size();
		const size_t height = grid.size();

		uint64_t highest_scenic_score = 0;
		for (size_t y = 1; y < height - 1; ++y)
		{
			for (size_t x = 1; x < width - 1; ++x)
			{
				uint64_t score = 1;
				constexpr std::array<std::array<int, 2>, 4> deltas = { {{1, 0}, {-1, 0}, {0, 1}, {0, -1}} };
				for (const auto& d : deltas)
				{
					const auto [_, dir_score] = check_tree(grid, x, y, d[0], d[1]);
					score *= dir_score;
				}

				highest_scenic_score = std::max(score, highest_scenic_score);
			}
		}

		return fmt::format("Day 8 Part 2 | Highest scenic score: {}", highest_scenic_score);
	}

} // aoc
