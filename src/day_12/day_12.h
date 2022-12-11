#ifndef AOC_2022_DAY_12_H
#define AOC_2022_DAY_12_H

#include "utility/day.h"

#include <filesystem>
#include <string>

namespace aoc
{
	class Day_12 : public Day
	{
		virtual std::string part_1(const std::filesystem::path& input_path) const override;
		virtual std::string part_2(const std::filesystem::path& input_path) const override;
	};

} // aoc

#endif // !AOC_2022_DAY_12_H
