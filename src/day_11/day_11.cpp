#include "day_11/day_11.h"

#include "day_11/monkey.h"
#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace aoc
{
	static std::vector<Monkey> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::vector<Monkey> monkeys;
		uint64_t divisor_product = 1;
		while (file)
		{
			Monkey monkey;
			file >> monkey;
			divisor_product *= monkey.get_test().get_divisor();
			monkeys.push_back(monkey);
		}

		// Set divisor product
		for (Monkey& monkey : monkeys)
		{
			monkey.set_divisor_product(divisor_product);
		}

		return monkeys;
	}

	std::string Day_11::part_1(const std::filesystem::path& input_path) const
	{
		std::vector<Monkey> monkeys = parse_input(input_path / "day_11.txt");
		for (int i = 0; i < 20; ++i)
		{
			for (Monkey& monkey : monkeys)
			{
				monkey.inspect_items(monkeys, true);
			}
		}

		std::sort(monkeys.begin(), monkeys.end(),
			[](const Monkey& lhs, const Monkey& rhs) { return lhs.inspected_items() > rhs.inspected_items(); });
		uint64_t monkey_business = monkeys[0].inspected_items() * monkeys[1].inspected_items();

		return fmt::format("Day 11 Part 1 | Level of monkey business after 20 rounds: {}", monkey_business);
	}

	std::string Day_11::part_2(const std::filesystem::path& input_path) const
	{
		std::vector<Monkey> monkeys = parse_input(input_path / "day_11.txt");
		for (int i = 0; i < 10000; ++i)
		{
			for (Monkey& monkey : monkeys)
			{
				monkey.inspect_items(monkeys, false);
			}
		}

		std::sort(monkeys.begin(), monkeys.end(),
			[](const Monkey& lhs, const Monkey& rhs) { return lhs.inspected_items() > rhs.inspected_items(); });
		uint64_t monkey_business = monkeys[0].inspected_items() * monkeys[1].inspected_items();

		return fmt::format("Day 11 Part 2 | Level of monkey business after 10000 rounds: {}", monkey_business);
	}

} // aoc
