#include "day_3/day_3.h"

#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <cstddef>

namespace aoc
{
	static std::vector<std::string> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = aoc::open_file(path);

		std::vector<std::string> rucksacks;
		for (std::string rucksack_items; file >> rucksack_items; )
		{
			rucksacks.push_back(rucksack_items);
		}

		return rucksacks;
	}

	static int item_priority(const char item)
	{
		return std::islower(item) ? (item - 97) + 1 : std::isupper(item) ? (item - 39) + 1 : 0;
	}

	std::string Day_3::part_1(const std::filesystem::path& input_path) const
	{
		std::vector<std::string> rucksacks = parse_input(input_path / "day_3.txt");

		int priority_total = 0;
		for (const std::string& rucksack_items : rucksacks)
		{
			const size_t slot_size = rucksack_items.length() / 2;
			const std::string slot_1 = rucksack_items.substr(0, slot_size);
			const std::string slot_2 = rucksack_items.substr(slot_size, slot_size);

			for (const char item : slot_1)
			{
				if (slot_2.find_first_of(item) != std::string::npos)
				{
					priority_total += item_priority(item);
					break;
				}
			}
		}

		return fmt::format("Day 3 Part 1 | Total item priorities: {}", priority_total);
	}

	std::string Day_3::part_2(const std::filesystem::path& input_path) const
	{
		std::vector<std::string> rucksacks = parse_input(input_path / "day_3.txt");

		int priority_total = 0;
		for (size_t i = 2; i < rucksacks.size(); i += 3)
		{
			const std::string& rucksack_1 = rucksacks[i];
			const std::string& rucksack_2 = rucksacks[i - 1];
			const std::string& rucksack_3 = rucksacks[i - 2];

			for (const char item : rucksack_1)
			{
				if (rucksack_2.find_first_of(item) != std::string::npos
					&& rucksack_3.find_first_of(item) != std::string::npos)
				{
					priority_total += item_priority(item);
					break;
				}
			}
		}

		return fmt::format("Day 3 Part 2 | Total item priorities: {}", priority_total);
	}

} // aoc
