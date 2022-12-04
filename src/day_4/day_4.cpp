#include "day_4/day_4.h"

#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <vector>
#include <utility>

namespace aoc
{
	struct Assignment
	{
		std::pair<int, int> section_1;
		std::pair<int, int> section_2;
	};

	static std::vector<Assignment> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = aoc::open_file(path);

		std::vector<Assignment> assignments;
		std::pair<int, int> section_1;
		std::pair<int, int> section_2;
		char junk = ' ';
		for (std::string line; std::getline(file, line); )
		{
			std::istringstream line_stream(line);
			line_stream >> section_1.first >> junk >> section_1.second
				>> junk >> section_2.first >> junk >> section_2.second;

			assignments.emplace_back(section_1, section_2);
		}

		return assignments;
	}

	std::string Day_4::part_1(const std::filesystem::path& input_path) const
	{
		std::vector<Assignment> assignments = parse_input(input_path / "day_4.txt");

		int fully_contained_assignments = 0;
		for (const Assignment& assignment : assignments)
		{
			const auto& section_1 = assignment.section_1;
			const auto& section_2 = assignment.section_2;

			if ((section_1.first >= section_2.first && section_1.second <= section_2.second)
				|| (section_2.first >= section_1.first && section_2.second <= section_1.second))
			{
				++fully_contained_assignments;
			}
		}

		return fmt::format("Day 4 Part 1 | Assignments with a fully contained range: {}", fully_contained_assignments);
	}

	std::string Day_4::part_2(const std::filesystem::path& input_path) const
	{
		std::vector<Assignment> assignments = parse_input(input_path / "day_4.txt");

		int overlapped_assignments = 0;
		for (const Assignment& assignment : assignments)
		{
			const auto& section_1 = assignment.section_1;
			const auto& section_2 = assignment.section_2;

			if (section_1.first <= section_2.second && section_2.first <= section_1.second)
			{
				++overlapped_assignments;
			}
		}

		return fmt::format("Day 4 Part 2 | Assignments with overlapped ranges: {}", overlapped_assignments);
	}

} // aoc
