#include "day_16/day_16.h"

#include "utility/io.h"
#include "utility/string.h"
#include "utility/utility.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <tuple>
#include <algorithm>
#include <cstdint>

namespace aoc
{
	struct Valve
	{
		std::string name;
		int flow_rate = 0;
		std::vector<std::string> connections;
	};

	static std::pair<std::unordered_map<std::string, Valve>, std::unordered_map<std::string, Valve>> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::unordered_map<std::string, Valve> valves;
		std::unordered_map<std::string, Valve> pressurized_valves;
		for (std::string line; std::getline(file, line) && !line.empty(); )
		{
			std::istringstream line_stream(line);

			line_stream.ignore(6);
			std::string valve_name;
			line_stream >> valve_name;

			line_stream.ignore(15);
			int flow_rate = 0;
			line_stream >> flow_rate;

			// `; tunnels lead to valves TU, AA`
			// `; tunnel leads to valve SA`

			line_stream.ignore(8);
			if (line_stream.peek() == 's')
				line_stream.ignore(17);
			else
				line_stream.ignore(16);

			std::string raw_connections;
			std::getline(line_stream, raw_connections);

			// Create map entry and add it to the maps
			Valve v{ valve_name, flow_rate };
			for (const std::string& connection : regex_split(raw_connections, ", "))
			{
				v.connections.push_back(connection);
			}

			valves[valve_name] = v;
			if (flow_rate > 0)
			{
				pressurized_valves[valve_name] = v;
			}
		}

		return std::make_pair(valves, pressurized_valves);
	}

	static std::unordered_map<std::pair<std::string, std::string>, int> create_shortest_paths(const std::unordered_map<std::string, Valve>& valves)
	{
		// Generate the shortest paths between all valves using the Floyd–Warshall algorithm
		// https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
		//
		// This will allow us to quickly jump between valves that will actually affect the pressure

		std::unordered_map<std::pair<std::string, std::string>, int> shortest_paths;
		for (const auto& [u, valve] : valves)
		{
			for (const auto& v : valve.connections)
			{
				shortest_paths[{ u, v }] = 1;
			}

			shortest_paths[{ u, u }] = 0;
		}

		for (const auto& [k, _1] : valves)
		{
			for (const auto& [i, _2] : valves)
			{
				if (!shortest_paths.contains({ i, k }))
					continue;

				for (const auto& [j, _3] : valves)
				{
					if (!shortest_paths.contains({ k, j }))
						continue;

					if (shortest_paths.contains({ i, j }))
						shortest_paths[{ i, j }] = std::min(shortest_paths[{ i, j }], shortest_paths[{ i, k }] + shortest_paths[{ k, j }]);
					else
						shortest_paths[{ i, j }] = shortest_paths[{ i, k }] + shortest_paths[{ k, j }];
				}
			}
		}

		return shortest_paths;
	}

	int64_t get_highest_pressure(
		const int total_time,
		const std::string& current_valve,
		const int current_time,
		std::set<std::string>& opened_valves_cache,
		const std::unordered_map<std::string, Valve>& pressurized_valves,
		const std::unordered_map<std::pair<std::string, std::string>, int>& shortest_paths)
	{
		// Return if all valves have been opened
		if (opened_valves_cache.size() == pressurized_valves.size())
			return 0;

		// Find the highest pressure from this valve
		int64_t highest_pressure = 0;
		for (const auto& [dest_name, dest_valve] : pressurized_valves)
		{
			// Skip this valve if it has already been opened
			if (opened_valves_cache.contains(dest_name))
				continue;

			// Get the time needed to open this valve and travel to the dest one
			// Also skip this valve if we run out of time
			int next_time = current_time + shortest_paths.at({ current_valve, dest_name }) + 1;
			if (next_time > total_time)
				continue;

			// Get highest pressure for this path
			opened_valves_cache.insert(dest_name);
			int64_t sub_pressure = get_highest_pressure(total_time, dest_name, next_time, opened_valves_cache, pressurized_valves, shortest_paths);
			opened_valves_cache.erase(dest_name);

			// Compare this pressure with the current highest
			int64_t released_pressure = dest_valve.flow_rate * (total_time - next_time);
			highest_pressure = std::max(highest_pressure, released_pressure + sub_pressure);
		}

		return highest_pressure;
	}

	int64_t get_highest_pressure_with_elephant(
		const int total_time,
		const std::string& current_valve,
		const std::string& current_elephant_valve,
		const int current_time,
		const int current_elephant_time,
		std::set<std::string>& opened_valves_cache,
		std::unordered_map<std::tuple<std::string, int, std::string, int, std::string>, int64_t>& state_cache,
		const std::unordered_map<std::string, Valve>& pressurized_valves,
		const std::unordered_map<std::pair<std::string, std::string>, int>& shortest_paths)
	{
		// Return if all valves have been opened
		if (opened_valves_cache.size() == pressurized_valves.size())
			return 0;

		// Create state cache keys
		const std::string opened = fmt::format("{}", fmt::join(opened_valves_cache, ""));
		const auto key_1 = std::make_tuple(current_valve, current_time, current_elephant_valve, current_time, opened);
		const auto key_2 = std::make_tuple(current_elephant_valve, current_elephant_time, current_valve, current_time, opened);

		// Return return cached pressure if this state already exists
		const auto key_1_it = state_cache.find(key_1);
		if (key_1_it != state_cache.end())
			return key_1_it->second;

		const auto key_2_it = state_cache.find(key_2);
		if (key_2_it != state_cache.end())
			return key_2_it->second;

		// Find the highest pressure from this valve
		int64_t highest_pressure = 0;
		for (const auto& [dest_name, dest_valve] : pressurized_valves)
		{
			// Skip this valve if it has already been opened
			if (opened_valves_cache.contains(dest_name))
				continue;

			// Get the time needed to open this valve and travel to the dest one
			int next_time = current_time + shortest_paths.at({ current_valve, dest_name }) + 1;
			int next_elephant_time = current_elephant_time + shortest_paths.at({ current_elephant_valve, dest_name }) + 1;

			if (next_time <= next_elephant_time && next_time <= total_time)
			{
				// Get highest pressure for this path
				opened_valves_cache.insert(dest_name);
				int64_t sub_pressure = get_highest_pressure_with_elephant(
					total_time, dest_name, current_elephant_valve, next_time, current_elephant_time,
					opened_valves_cache, state_cache, pressurized_valves, shortest_paths);
				opened_valves_cache.erase(dest_name);

				// Compare this pressure with the current highest
				int64_t released_pressure = dest_valve.flow_rate * (total_time - next_time);
				highest_pressure = std::max(highest_pressure, released_pressure + sub_pressure);
			}

			if (next_elephant_time <= next_time && next_elephant_time <= total_time)
			{
				// Get highest pressure for this path
				opened_valves_cache.insert(dest_name);
				int64_t sub_pressure = get_highest_pressure_with_elephant(
					total_time, current_valve, dest_name, current_time, next_elephant_time,
					opened_valves_cache, state_cache, pressurized_valves, shortest_paths);
				opened_valves_cache.erase(dest_name);

				// Compare this pressure with the current highest
				int64_t released_pressure = dest_valve.flow_rate * (total_time - next_elephant_time);
				highest_pressure = std::max(highest_pressure, released_pressure + sub_pressure);
			}

		}

		// Add this state to the state cache
		state_cache[key_1] = highest_pressure;
		state_cache[key_2] = highest_pressure;

		return highest_pressure;
	}

	std::string Day_16::part_1(const std::filesystem::path& input_path) const
	{
		const auto [valves, pressurized_valves] = parse_input(input_path / "day_16.txt");
		const auto shortest_paths = create_shortest_paths(valves);

		std::set<std::string> opened_valves_cache;
		const int64_t highest_pressure = get_highest_pressure(30, "AA", 0, opened_valves_cache, pressurized_valves, shortest_paths);

		return fmt::format("Day 16 Part 1 | Highest pressure you can release: {}", highest_pressure);
	}

	std::string Day_16::part_2(const std::filesystem::path& input_path) const
	{
		const auto [valves, pressurized_valves] = parse_input(input_path / "day_16.txt");
		const auto shortest_paths = create_shortest_paths(valves);

		std::set<std::string> opened_valves_cache;
		std::unordered_map<std::tuple<std::string, int, std::string, int, std::string>, int64_t> state_cache;
		const int64_t highest_pressure = get_highest_pressure_with_elephant(26, "AA", "AA", 0, 0, opened_valves_cache, state_cache, pressurized_valves, shortest_paths);

		return fmt::format("Day 16 Part 2 | Highest pressure you can release with the help of an elephant: {}", highest_pressure);
	}

} // aoc
