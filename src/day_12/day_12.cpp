#include "day_12/day_12.h"

#include "utility/io.h"
#include "utility/point.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <limits>
#include <utility>

namespace aoc
{
	static std::vector<std::vector<char>> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::vector<std::vector<char>> grid;
		for (std::string line; std::getline(file, line); )
		{
			std::vector<char> columns;
			for (const char c : line)
			{
				columns.push_back(c);
			}

			grid.push_back(columns);
		}

		return grid;
	}

	static Point find_first_of_char(const std::vector<std::vector<char>>& grid, const char c)
	{
		for (size_t y = 0; y < grid.size(); ++y)
		{
			for (size_t x = 0; x < grid.front().size(); ++x)
			{
				if (grid[y][x] == c)
				{
					return Point(x, y);
				}
			}
		}

		return Point::origin;
	}

	static bool on_grid(const std::vector<std::vector<char>>& grid, const Point& p)
	{
		const size_t max_x = grid.front().size() - 1;
		const size_t max_y = grid.size() - 1;

		return p.x >= 0 && p.x <= max_x && p.y >= 0 && p.y <= max_y;
	}

	static bool adjacent_to(const std::vector<std::vector<char>>& grid, const Point& p, const char c)
	{
		for (const Point& adj : p.adjacent_cardinal())
		{
			if (on_grid(grid, adj) && grid[adj.y][adj.x] == c)
			{
				return true;
			}
		}

		return false;
	}

	static int shortest_path(const std::vector<std::vector<char>>& grid, const Point& start, const Point& end)
	{
		std::queue<std::pair<Point, int>> to_process;
		to_process.emplace(start, 0);
		std::unordered_set<Point> visited;
		while (!to_process.empty())
		{
			const auto [current, steps] = to_process.front();
			to_process.pop();

			// Skip this point if it has already been processed
			if (visited.contains(current))
				continue;

			// Return if we found the end point
			if (current == end)
				return steps;

			// Add this point to the visited set
			visited.insert(current);

			// Expand search to nearby points
			for (const Point& adj : current.adjacent_cardinal())
			{
				if (!on_grid(grid, adj) || visited.contains(adj))
					continue;

				char current_height = grid[current.y][current.x];
				char adj_height = grid[adj.y][adj.x];

				// a = 0, b = 1, c = 2
				// aa (a - a) -> 0 | ab (b - a) -> 1 | ac (c - a) -> 2 | ba (a - b) -> -1 
				if (grid[adj.y][adj.x] - grid[current.y][current.x] > 1)
					continue;

				to_process.emplace(adj, steps + 1);
			}
		}

		return std::numeric_limits<int>::min();
	}

	std::string Day_12::part_1(const std::filesystem::path& input_path) const
	{
		std::vector<std::vector<char>> grid = parse_input(input_path / "day_12.txt");

		const Point start = find_first_of_char(grid, 'S');
		const Point end = find_first_of_char(grid, 'E');

		grid[start.y][start.x] = 'a';
		grid[end.y][end.x] = 'z';

		int fewest_steps = shortest_path(grid, start, end);

		return fmt::format("Day 12 Part 1 | Fewest steps required to get the best signal: {}", fewest_steps);
	}

	std::string Day_12::part_2(const std::filesystem::path& input_path) const
	{
		std::vector<std::vector<char>> grid = parse_input(input_path / "day_12.txt");

		const Point start = find_first_of_char(grid, 'S');
		const Point end = find_first_of_char(grid, 'E');

		grid[start.y][start.x] = 'a';
		grid[end.y][end.x] = 'z';

		int fewest_steps = std::numeric_limits<int>::max();
		for (size_t y = 0; y < grid.size(); ++y)
		{
			for (size_t x = 0; x < grid.front().size(); ++x)
			{
				if (grid[y][x] != 'a')
					continue;

				if (!adjacent_to(grid, Point(x, y), 'b'))
					continue;

				int steps = shortest_path(grid, Point(x, y), end);
				fewest_steps = (steps >= 0) ? std::min(fewest_steps, steps) : fewest_steps;
			}
		}

		return fmt::format("Day 12 Part 2 | Fewest steps required from any 'a' to get the best signal: {}", fewest_steps);
	}

} // aoc
