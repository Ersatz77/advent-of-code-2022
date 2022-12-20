#include "day_20/day_20.h"

#include "utility/io.h"
#include "utility/math.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <cstdint>

namespace aoc
{
	using Iterator = std::list<int64_t>::iterator;

	static std::list<int64_t> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::list<int64_t> numbers;
		for (int64_t value = 0; file >> value; )
		{
			numbers.push_back(value);
		}

		return numbers;
	}

	static std::vector<Iterator> generate_order(std::list<int64_t>& numbers)
	{
		std::vector<Iterator> order;
		for (Iterator it = numbers.begin(); it != numbers.end(); ++it)
		{
			order.push_back(it);
		}

		return order;
	}

	static void mix(std::list<int64_t>& numbers, std::vector<Iterator>& order, const int64_t iterations)
	{
		for (int64_t i = 0; i < iterations; ++i)
		{
			for (Iterator& current_it : order)
			{
				// Get a copy of the current number
				int64_t value = *current_it;

				// Get the new index for this number (We use `size - 1` since we'll be removing an element temporarily)
				// The first index also becomes the last index
				int64_t new_index = euclid_mod((int64_t)std::distance(numbers.begin(), current_it) + value, (int64_t)numbers.size() - 1);
				new_index = (new_index == 0) ? numbers.size() - 1 : new_index;

				// Remove value at the current iterator (this will invalidate `current_it`)
				numbers.erase(current_it);

				// Move an iterator to the new index
				Iterator new_it = numbers.begin();
				std::advance(new_it, new_index);

				// Insert element and update iterator
				current_it = numbers.insert(new_it, value);
			}
		}
	}

	static int64_t get_coord(const std::list<int64_t>& numbers, const int64_t n_after_zero)
	{
		auto it = numbers.begin();
		while (it != numbers.end() && *it != 0)
		{
			++it;
		}

		int64_t current_index = std::distance(numbers.begin(), it);
		int64_t to_advance = euclid_mod((int64_t)current_index + n_after_zero, (int64_t)numbers.size()) - current_index;
		std::advance(it, to_advance);

		return *it;
	}

	std::string Day_20::part_1(const std::filesystem::path& input_path) const
	{
		std::list<int64_t> numbers = parse_input(input_path / "day_20.txt");
		std::vector<Iterator> order = generate_order(numbers);

		mix(numbers, order, 1);
		int64_t coordinate_sum = get_coord(numbers, 1000) + get_coord(numbers, 2000) + get_coord(numbers, 3000);

		return fmt::format("Day 20 Part 1 | Grove coordinate sum: {}", coordinate_sum);
	}

	std::string Day_20::part_2(const std::filesystem::path& input_path) const
	{
		std::list<int64_t> numbers = parse_input(input_path / "day_20.txt");
		std::vector<Iterator> order = generate_order(numbers);

		// Apply decryption key
		constexpr int64_t decryption_key = 811589153;
		for (int64_t& value : numbers)
		{
			value *= decryption_key;
		}

		mix(numbers, order, 10);
		int64_t coordinate_sum = get_coord(numbers, 1000) + get_coord(numbers, 2000) + get_coord(numbers, 3000);

		return fmt::format("Day 20 Part 2 | Real grove coordinate sum: {}", coordinate_sum);
	}

} // aoc
