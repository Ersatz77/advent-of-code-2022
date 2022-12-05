#include "day_6/day_6.h"

#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace aoc
{
	std::string Day_6::part_1(const std::filesystem::path& input_path) const
	{
		return fmt::format("Day 6 Part 1 | {}", input_path.string());
	}

	std::string Day_6::part_2(const std::filesystem::path& input_path) const
	{
		return fmt::format("Day 6 Part 2 | {}", input_path.string());
	}

} // aoc
