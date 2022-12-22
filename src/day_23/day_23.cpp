#include "day_23/day_23.h"

#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace aoc
{
	std::string Day_23::part_1(const std::filesystem::path& input_path) const
	{
		return fmt::format("Day 23 Part 1 | {}", input_path.string());
	}

	std::string Day_23::part_2(const std::filesystem::path& input_path) const
	{
		return fmt::format("Day 23 Part 2 | {}", input_path.string());
	}

} // aoc
