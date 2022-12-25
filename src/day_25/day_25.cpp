#include "day_25/day_25.h"

#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <cmath>
#include <cstdint>

namespace aoc
{
	static std::vector<std::string> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::vector<std::string> snafu_numbers;
		for (std::string snafu_number; file >> snafu_number; )
		{
			snafu_numbers.push_back(snafu_number);
		}

		return snafu_numbers;
	}

	static int64_t decode_snafu_digit(const char s)
	{
		switch (s)
		{
			case '-':
				return -1;
			case '=':
				return -2;
			default:
				return static_cast<int64_t>(s) - '0';
		}
	}

	static int64_t from_snafu(const std::string& snafu)
	{
		int64_t value = 0;
		for (int64_t i = 0; i < snafu.length(); ++i)
		{
			int64_t digit_value = std::pow(5, (snafu.length() - 1) - i);
			value += decode_snafu_digit(snafu[i]) * digit_value;
		}

		return value;
	}

	static std::string to_snafu(int64_t value)
	{
		std::string snafu;
		while (value > 0)
		{
			int64_t digit = value % 5;
			value /= 5;

			switch (digit)
			{
				case 0:
					snafu = '0' + snafu;
					break;
				case 1:
					snafu = '1' + snafu;
					break;
				case 2:
					snafu = '2' + snafu;
					break;
				case 3:
					snafu = '=' + snafu;
					++value;
					break;
				case 4:
					snafu = '-' + snafu;
					++value;
					break;
			}
		}

		return snafu;
	}

	std::string Day_25::part_1(const std::filesystem::path& input_path) const
	{
		std::vector<std::string> snafu_numbers = parse_input(input_path / "day_25.txt");

		// Get the total amount of fuel required in decimal
		int64_t total_fuel_decimal = 0;
		for (const std::string& snafu_number : snafu_numbers)
		{
			total_fuel_decimal += from_snafu(snafu_number);
		}

		// Get the total amount of fuel required in snafu
		std::string total_fuel_snafu = to_snafu(total_fuel_decimal);

		return fmt::format("Day 25 Part 1 | Total fuel for all the hot air balloons: {}", total_fuel_snafu);
	}

	std::string Day_25::part_2(const std::filesystem::path& input_path) const
	{
		return fmt::format("Day 25 Part 2 | You start the blender and make a smoothie for the reindeer");
	}

} // aoc
