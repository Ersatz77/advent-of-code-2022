#include "day_1/day_1.h"

#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

namespace aoc
{
	static std::vector<int> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = aoc::open_file(path);

		std::vector<int> input;
		while (file)
		{
			int total = 0;
			for (std::string line; std::getline(file, line) && !line.empty(); )
			{
				total += std::stoi(line);
			}

			input.push_back(total);
		}

		return input;
	}

	std::string Day_1::part_1(const std::filesystem::path& input_path) const
	{
		std::vector<int> input = parse_input(input_path / "day_1.txt");

		return fmt::format("Day 1 Part 1 | Greatest total calories carried by an elf: {}", *std::max_element(input.begin(), input.end()));
	}

	std::string Day_1::part_2(const std::filesystem::path& input_path) const
	{
		std::vector<int> input = parse_input(input_path / "day_1.txt");	
		
		std::sort(input.begin(), input.end());
		int total = std::accumulate(input.rbegin(), input.rbegin() + 3, 0);

		return fmt::format("Day 1 Part 2 | Total calories carried by the top 3 elves: {}", total);
	}

} // aoc
