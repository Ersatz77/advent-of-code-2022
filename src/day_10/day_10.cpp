#include "day_10/day_10.h"

#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <functional>
#include <numeric>
#include <cstdint>

namespace aoc
{
	enum class CPU_operation
	{
		NOOP = 0,
		ADD
	};

	struct CPU_instruction
	{
		CPU_operation op;
		int64_t value;
	};

	static std::vector<CPU_instruction> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::vector<CPU_instruction> instructions;
		std::string op_str;
		int64_t value = 0;
		while (file >> op_str)
		{
			if (op_str == "noop")
			{
				instructions.emplace_back(CPU_operation::NOOP, 0);
			}
			else if (op_str == "addx")
			{
				file >> value;
				instructions.emplace_back(CPU_operation::ADD, value);
			}
		}

		return instructions;
	}

	static std::vector<int64_t> run_program(const std::vector<CPU_instruction>& instructions, const std::function<int64_t(int64_t, int64_t)> func)
	{
		std::vector<int64_t> results;
		auto ip = instructions.begin();
		int64_t cycle = 1;
		int64_t reg_x = 1;
		while (ip != instructions.end())
		{
			results.push_back(func(reg_x, cycle));

			if (ip->op == CPU_operation::ADD)
			{
				++cycle;
				results.push_back(func(reg_x, cycle));
				reg_x += ip->value;
			}

			++ip;
			++cycle;
		}

		return results;
	}

	static int64_t get_signal_strength(const int64_t reg_x, const int64_t cycle)
	{
		return (cycle + 20) % 40 == 0 ? reg_x * cycle : 0;
	}

	static int64_t get_pixel(const int64_t reg_x, const int64_t cycle)
	{
		int64_t x = (cycle - 1) % 40;
		return (x == reg_x - 1 || x == reg_x || x == reg_x + 1) ? 1LL : 0LL;
	}

	std::string Day_10::part_1(const std::filesystem::path& input_path) const
	{
		std::vector<CPU_instruction> instructions = parse_input(input_path / "day_10.txt");

		std::vector<int64_t> signal_strengths = run_program(instructions, get_signal_strength);
		int64_t signal_strength_sum = std::accumulate(signal_strengths.begin(), signal_strengths.end(), 0LL);

		return fmt::format("Day 10 Part 1 | Signal strength sum: {}", signal_strength_sum);
	}

	std::string Day_10::part_2(const std::filesystem::path& input_path) const
	{
		std::vector<CPU_instruction> instructions = parse_input(input_path / "day_10.txt");

		std::vector<int64_t> pixels = run_program(instructions, get_pixel);

		std::string output;
		constexpr size_t width = 40;
		constexpr size_t height = 6;
		for (size_t y = 0; y < height; ++y)
		{
			for (size_t x = 0; x < width; ++x)
			{
				int64_t pixel = pixels[y * width + x];
				output += pixel == 1 ? '#' : ' ';
			}

			if (y < height - 1)
			{
				output += '\n';
			}
		}

		return fmt::format("Day 10 Part 2 |\n{}", output);
	}

} // aoc
