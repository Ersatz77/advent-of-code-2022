#include "day_1/day_1.h"

#include "utility//io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace aoc
{
	std::string Day_1::part_1(const std::filesystem::path& input_path) const
	{
		return fmt::format("{}", input_path.string());
	}

	std::string Day_1::part_2(const std::filesystem::path& input_path) const
	{
		return fmt::format("{}", input_path.string());
	}

} // aoc
