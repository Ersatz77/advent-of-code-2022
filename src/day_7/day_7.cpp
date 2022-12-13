#include "day_7/day_7.h"

#include "day_7/filesystem.h"
#include "utility/io.h"
#include "utility/string.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

namespace aoc
{
	static Directory parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		Directory root(nullptr, "/");
		Directory* current = &root;
		for (std::string line; std::getline(file, line); )
		{
			std::vector<std::string> parts = split(line, ' ');
			// Process commands
			if (parts[0] == "$")
			{
				if (parts[1] == "cd" && parts[1] == "/")
				{
					current = &root;
				}
				else if (parts[1] == "cd" && parts[2] == "..")
				{
					current = current->parent();
				}
				else if (parts[1] == "cd")
				{
					for (auto& child_dir : current->directories())
					{
						if (child_dir.name() == parts[2])
						{
							current = &child_dir;
							break;
						}
					}
				}

				// This will also skip '$ ls'
				continue;
			}

			// Modify current directory
			if (parts[0] == "dir")
			{
				// Add directory
				current->add_directory(parts[1]);
			}
			else
			{
				// Add file
				size_t size = std::stoi(parts[0]);
				current->add_file(parts[1], size);
			}
		}

		return root;
	}

	static std::vector<size_t> get_directory_sizes(const Directory& filesystem)
	{
		std::vector<size_t> sizes;
		std::queue<Directory> to_process({ filesystem });
		while (!to_process.empty())
		{
			Directory directory = to_process.front();
			to_process.pop();

			sizes.push_back(directory.size());

			for (const auto& child_directory : directory.directories())
			{
				to_process.push(child_directory);
			}
		}

		return sizes;
	}

	std::string Day_7::part_1(const std::filesystem::path& input_path) const
	{
		Directory filesystem = parse_input(input_path / "day_7.txt");

		const std::vector<size_t> sizes = get_directory_sizes(filesystem);

		constexpr size_t max_size = 100000;
		size_t total_sizes = 0;
		for (const size_t i : sizes)
		{
			if (i <= max_size)
			{
				total_sizes += i;
			}
		}

		return fmt::format("Day 7 Part 1 | Total size of all directories under {}: {}", max_size, total_sizes);
	}

	std::string Day_7::part_2(const std::filesystem::path& input_path) const
	{
		Directory filesystem = parse_input(input_path / "day_7.txt");

		const std::vector<size_t> sizes = get_directory_sizes(filesystem);

		const size_t available_space = 70000000 - filesystem.size();
		const size_t min_space_to_remove = 30000000 - available_space;
		size_t to_delete = std::numeric_limits<size_t>::max();
		for (const size_t i : sizes)
		{
			if (i >= min_space_to_remove)
			{
				to_delete = std::min(to_delete, i);
			}
		}

		return fmt::format("Day 7 Part 2 | Size to delete to allow for an update to happen: {}", to_delete);
	}

} // aoc
