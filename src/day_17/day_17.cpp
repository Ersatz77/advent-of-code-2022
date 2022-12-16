#include "day_17/day_17.h"

#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

namespace aoc
{
	std::string Day_17::part_1(const std::filesystem::path& input_path) const
	{
		return fmt::format("Day 17 Part 1 | {}", input_path.string());
	}

	std::string Day_17::part_2(const std::filesystem::path& input_path) const
	{
		return fmt::format("Day 17 Part 2 | {}", input_path.string());
	}

} // aoc
