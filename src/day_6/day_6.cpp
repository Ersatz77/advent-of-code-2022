#include "day_6/day_6.h"

#include "utility/io.h"
#include "utility/string.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace aoc
{
	static std::string parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = aoc::open_file(path);

		std::string buffer;
		file >> buffer;

		return buffer;
	}

	std::string Day_6::part_1(const std::filesystem::path& input_path) const
	{
		std::string buffer = parse_input(input_path / "day_6.txt");

		size_t start_of_packet = 0;
		for (size_t i = 4; i < buffer.size(); ++i)
		{
			std::string buffer_part(buffer.begin() + i - 4, buffer.begin() + i);
			if (aoc::is_unique(buffer_part))
			{
				start_of_packet = i;
				break;
			}
		}

		return fmt::format("Day 6 Part 1 | Characters processed until the start of a packet: {}", start_of_packet);
	}

	std::string Day_6::part_2(const std::filesystem::path& input_path) const
	{
		std::string buffer = parse_input(input_path / "day_6.txt");

		size_t start_of_message = 0;
		for (size_t i = 14; i < buffer.size(); ++i)
		{
			std::string buffer_part(buffer.begin() + i - 14, buffer.begin() + i);
			if (aoc::is_unique(buffer_part))
			{
				start_of_message = i;
				break;
			}
		}

		return fmt::format("Day 6 Part 2 | Characters processed until the start of a message: {}", start_of_message);
	}

} // aoc
