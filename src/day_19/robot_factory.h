#ifndef AOC_2022_DAY_19_ROBOT_FACTORY_H
#define AOC_2022_DAY_19_ROBOT_FACTORY_H

#include "utility/utility.h"

#include <iostream>
#include <compare>

namespace aoc
{
	struct Robot_blueprint
	{
		friend std::ostream& operator<<(std::ostream& os, const Robot_blueprint& r);

		int required_ore;
		int required_clay;
		int required_obsidian;
	};

	struct Factory_blueprint
	{
		int max_ore_cost() const;
		friend std::ostream& operator<<(std::ostream& os, const Factory_blueprint& b);

		int id;
		Robot_blueprint ore_robot;
		Robot_blueprint clay_robot;
		Robot_blueprint obsidian_robot;
		Robot_blueprint geode_robot;
	};

	struct Factory_state
	{
		void mine_resources();
		void start_building_robot(const Robot_blueprint& robot_blueprint);
		bool can_build(const Robot_blueprint& robot_blueprint) const;

		std::strong_ordering operator<=>(const Factory_state& other) const = default;

		int time;

		int ore;
		int clay;
		int obsidian;
		int geodes;

		int ore_robots;
		int clay_robots;
		int obsidian_robots;
		int geode_robots;
	};

} // aoc

namespace std
{
	template <>
	struct hash<aoc::Factory_state>
	{
		std::size_t operator()(const aoc::Factory_state& s) const
		{
			size_t seed = 0;
			aoc::hash_combine(seed, s.time);
			aoc::hash_combine(seed, s.ore);
			aoc::hash_combine(seed, s.clay);
			aoc::hash_combine(seed, s.obsidian);
			aoc::hash_combine(seed, s.geodes);
			aoc::hash_combine(seed, s.ore_robots);
			aoc::hash_combine(seed, s.clay_robots);
			aoc::hash_combine(seed, s.obsidian_robots);
			aoc::hash_combine(seed, s.geode_robots);
			return seed;
		}
	};

} // std

#endif // !AOC_2022_DAY_19_ROBOT_FACTORY_H
