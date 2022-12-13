#include "day_13/day_13.h"

#include "day_13/list.h"
#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <compare>

namespace aoc
{
	static std::vector<List> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::vector<List> packets;
		char junk = ' ';
		while (file >> junk)
		{
			List packet = std::vector<List>{};
			parse_packet(file, packet);
			packets.push_back(packet);
		}

		return packets;
	}

	std::string Day_13::part_1(const std::filesystem::path& input_path) const
	{
		std::vector<List> packets = parse_input(input_path / "day_13.txt");

		size_t sum = 0;
		size_t idx = 0;
		for (size_t i = 1; i < packets.size(); i += 2)
		{
			if (packets[i - 1].compare(packets[i]) == std::weak_ordering::less)
			{
				sum += idx + 1;
			}

			++idx;
		}

		return fmt::format("Day 13 Part 1 | Sum of the indices of pairs that are in the right order: {}", sum);
	}

	std::string Day_13::part_2(const std::filesystem::path& input_path) const
	{
		std::vector<List> packets = parse_input(input_path / "day_13.txt");

		List divider_1 = std::vector<List>(std::vector<List>(1, 2));
		List divider_2 = std::vector<List>(std::vector<List>(1, 6));

		packets.push_back(divider_1);
		packets.push_back(divider_2);

		std::sort(packets.begin(), packets.end(),
			[](List& lhs, List& rhs) { return lhs.compare(rhs) == std::weak_ordering::less; });

		size_t key = 1;
		for (size_t i = 0; i < packets.size(); ++i)
		{
			if (packets[i].compare(divider_1) == std::weak_ordering::equivalent
				|| packets[i].compare(divider_2) == std::weak_ordering::equivalent)
			{
				key *= i + 1;
			}
		}

		return fmt::format("Day 13 Part 2 | Decoder key: {}", key);
	}

} // aoc
