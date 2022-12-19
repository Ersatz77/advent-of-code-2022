#include "day_19/day_19.h"

#include "day_19/robot_factory.h"
#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <ranges>

namespace aoc
{
	static std::vector<Factory_blueprint> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::vector<Factory_blueprint> blueprints;
		for (std::string line; std::getline(file, line); )
		{
			std::istringstream line_stream(line);

			int id = 0;
			Robot_blueprint ore_robot{};
			Robot_blueprint clay_robot{};
			Robot_blueprint obsidian_robot{};
			Robot_blueprint geode_robot{};

			line_stream.ignore(10);
			line_stream >> id;

			line_stream.ignore(23);
			line_stream >> ore_robot.required_ore;

			line_stream.ignore(28);
			line_stream >> clay_robot.required_ore;

			line_stream.ignore(32);
			line_stream >> obsidian_robot.required_ore;
			line_stream.ignore(9);
			line_stream >> obsidian_robot.required_clay;

			line_stream.ignore(30);
			line_stream >> geode_robot.required_ore;
			line_stream.ignore(9);
			line_stream >> geode_robot.required_obsidian;

			blueprints.emplace_back(id, ore_robot, clay_robot, obsidian_robot, geode_robot);
		}

		return blueprints;
	}

	static int run_factory(const Factory_blueprint& blueprint, const int64_t max_time)
	{
		int best_geodes = 0;
		int max_ore_cost = blueprint.max_ore_cost();

		std::unordered_set<Factory_state> seen;
		std::queue<Factory_state> to_process({ {0, 0, 0, 0, 0, 1, 0, 0, 0} });
		while (!to_process.empty())
		{
			Factory_state state = to_process.front();
			to_process.pop();

			best_geodes = std::max(best_geodes, state.geodes);
			if (state.geodes < best_geodes - 1 || !seen.insert(state).second)
				continue;

			// Don't process state further if we run out of time
			if (state.time == max_time)
				continue;

			// Add state where we build a geode robot
			if (state.can_build(blueprint.geode_robot))
			{
				Factory_state next = state;
				next.start_building_robot(blueprint.geode_robot);
				next.mine_resources();
				++next.geode_robots;
				to_process.push(next);
				continue;
			}
			
			// Add state where we build an ore robot
			if (state.can_build(blueprint.ore_robot) && state.ore_robots < max_ore_cost)
			{
				Factory_state next = state;
				next.start_building_robot(blueprint.ore_robot);
				next.mine_resources();
				++next.ore_robots;
				to_process.push(next);
			}

			// Add state where we build a clay robot
			if (state.can_build(blueprint.clay_robot))
			{
				Factory_state next = state;
				next.start_building_robot(blueprint.clay_robot);
				next.mine_resources();
				++next.clay_robots;
				to_process.push(next);
			}

			// Add state where we build an obsidian robot
			if (state.can_build(blueprint.obsidian_robot))
			{
				Factory_state next = state;
				next.start_building_robot(blueprint.obsidian_robot);
				next.mine_resources();
				++next.obsidian_robots;
				to_process.push(next);
			}

			// Add state where we only mine resources
			state.mine_resources();
			to_process.push(state);
		}
		
		return best_geodes;
	}

	std::string Day_19::part_1(const std::filesystem::path& input_path) const
	{
		std::vector<Factory_blueprint> blueprints = parse_input(input_path / "day_19.txt");
		constexpr int time = 24;
		int quality_level = 0;
		for (const Factory_blueprint& blueprint : blueprints)
		{
			quality_level += (run_factory(blueprint, time) * blueprint.id);
		}

		return fmt::format("Day 19 Part 1 | Blueprint quality level sum: {}", quality_level);
	}

	std::string Day_19::part_2(const std::filesystem::path& input_path) const
	{
		std::vector<Factory_blueprint> blueprints = parse_input(input_path / "day_19.txt");
		
		constexpr int time = 32;
		int geodes_found = 1;
		for (const Factory_blueprint& blueprint : blueprints | std::ranges::views::take(3))
		{
			geodes_found *= run_factory(blueprint, time);
		}

		return fmt::format("Day 19 Part 2 | Geodes collected by the first 3 blueprints: {}", geodes_found);
	}

} // aoc
