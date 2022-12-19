#include "day_19/robot_factory.h"

#include "fmt/format.h"

#include <iostream>
#include <algorithm>

namespace aoc
{
	// ------------------------------------------------------------------------------------
	// Robot Blueprint
	// ------------------------------------------------------------------------------------

	std::ostream& operator<<(std::ostream& os, const Robot_blueprint& r)
	{
		return os << fmt::format("Ore: {} Clay: {} Obsidian: {}", r.required_ore, r.required_clay, r.required_obsidian);
	}

	// ------------------------------------------------------------------------------------
	// Factory Blueprint
	// ------------------------------------------------------------------------------------

	int Factory_blueprint::max_ore_cost() const
	{
		int max_ore = ore_robot.required_ore;
		max_ore = std::max(max_ore, clay_robot.required_ore);
		max_ore = std::max(max_ore, obsidian_robot.required_ore);
		max_ore = std::max(max_ore, geode_robot.required_ore);
		return max_ore;
	}

	std::ostream& operator<<(std::ostream& os, const Factory_blueprint& b)
	{
		return os << "ID: " << b.id
			<< "\n\tOre robot cost: [" << b.ore_robot
			<< "]\n\tClay robot cost: [" << b.clay_robot
			<< "]\n\tObsidian robot cost: [" << b.obsidian_robot
			<< "]\n\tGeode robot cost: [" << b.geode_robot << ']';
	}

	// ------------------------------------------------------------------------------------
	// Factory State
	// ------------------------------------------------------------------------------------

	void Factory_state::mine_resources()
	{
		ore += ore_robots;
		clay += clay_robots;
		obsidian += obsidian_robots;
		geodes += geode_robots;
		++time;
	}

	void Factory_state::start_building_robot(const Robot_blueprint& robot_blueprint)
	{
		ore -= robot_blueprint.required_ore;
		clay -= robot_blueprint.required_clay;
		obsidian -= robot_blueprint.required_obsidian;
	}

	bool Factory_state::can_build(const Robot_blueprint& robot_blueprint) const
	{
		return ore >= robot_blueprint.required_ore
			&& clay >= robot_blueprint.required_clay
			&& obsidian >= robot_blueprint.required_obsidian;
	}

} // aoc
