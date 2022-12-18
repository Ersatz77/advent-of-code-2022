#include "day_18/day_18.h"

#include "utility/io.h"
#include "utility/point.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <unordered_set>
#include <stack>
#include <algorithm>
#include <limits>
#include <utility>
#include <cstdint>

namespace aoc
{ 
	static std::unordered_set<Point_3d> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::unordered_set<Point_3d> lava_droplets;
		int x = 0;
		int y = 0;
		int z = 0;
		char junk = ' ';
		while (file >> x >> junk >> y >> junk >> z)
		{
			lava_droplets.emplace(x, y, z);
		}

		return lava_droplets;
	}

	static std::pair<Point_3d, Point_3d> create_bounding_box(const std::unordered_set<Point_3d>& lava_droplets, const int extension = 1)
	{
		Point_3d min{ std::numeric_limits<double>::max(), std::numeric_limits<double>::max() ,std::numeric_limits<double>::max() };
		Point_3d max{ std::numeric_limits<double>::min(), std::numeric_limits<double>::min() ,std::numeric_limits<double>::min() };
		for (const Point_3d& droplet : lava_droplets)
		{
			min.x = std::min(min.x, droplet.x - extension);
			min.y = std::min(min.y, droplet.y - extension);
			min.z = std::min(min.z, droplet.z - extension);

			max.x = std::max(max.x, droplet.x + extension);
			max.y = std::max(max.y, droplet.y + extension);
			max.z = std::max(max.z, droplet.z + extension);
		}

		return std::make_pair(min, max);
	}

	static bool in_bounding_box(const Point_3d& droplet, const Point_3d& min, const Point_3d& max)
	{
		return droplet.x >= min.x && droplet.x <= max.x
			&& droplet.y >= min.y && droplet.y <= max.y
			&& droplet.z >= min.z && droplet.z <= max.z;
	}

	static int64_t surface_area(const std::unordered_set<Point_3d>& lava_droplets, const Point_3d& min, const Point_3d& max)
	{
		int64_t surface_area = 0;
		for (const Point_3d& droplet : lava_droplets)
		{
			for (const Point_3d& adj : droplet.adjacent_cardinal())
			{
				if (in_bounding_box(adj, min, max) && !lava_droplets.contains(adj))
				{
					++surface_area;
				}
			}
		}

		return surface_area;
	}

	static std::unordered_set<Point_3d> flood_fill_box(const std::unordered_set<Point_3d>& lava_droplets, const Point_3d& min, const Point_3d& max)
	{
		std::unordered_set<Point_3d> visited;
		std::stack<Point_3d> to_process({ min });
		while (!to_process.empty())
		{
			const Point_3d current = to_process.top();
			to_process.pop();

			if (!visited.insert(current).second)
				continue;

			for (const Point_3d& adj : current.adjacent_cardinal())
			{
				if (in_bounding_box(adj, min, max) && !lava_droplets.contains(adj))
				{
					to_process.push(adj);
				}
			}
		}

		return visited;
	}

	std::string Day_18::part_1(const std::filesystem::path& input_path) const
	{
		const std::unordered_set<Point_3d> lava_droplets = parse_input(input_path / "day_18.txt");
		const auto [min, max] = create_bounding_box(lava_droplets);

		int64_t area = surface_area(lava_droplets, min, max);
	
		return fmt::format("Day 18 Part 1 | Lava droplet surface area: {}", area);
	}

	std::string Day_18::part_2(const std::filesystem::path& input_path) const
	{
		const std::unordered_set<Point_3d> lava_droplets = parse_input(input_path / "day_18.txt");
		const auto [min, max] = create_bounding_box(lava_droplets);

		const std::unordered_set<Point_3d> inverted_lava_droplets = flood_fill_box(lava_droplets, min, max);
		int64_t area = surface_area(inverted_lava_droplets, min, max);

		return fmt::format("Day 18 Part 2 | Real lava droplet surface area: {}", area);
	}

} // aoc
