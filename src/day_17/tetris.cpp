#include "day_17/tetris.h"

#include "utility/point.h"

#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <unordered_set>
#include <algorithm>
#include <limits>
#include <compare>
#include <utility>
#include <cstdint>

namespace aoc
{
	// ------------------------------------------------------------------------------------
	// Utilities
	// ------------------------------------------------------------------------------------

	struct Tetris_state
	{
		std::array<int64_t, 7> profile;
		size_t jet_idx;
		int rock_shape;

		std::strong_ordering operator<=>(const Tetris_state& other) const = default;
	};

	// ------------------------------------------------------------------------------------
	// Rock
	// ------------------------------------------------------------------------------------

	Rock::Rock(const Rock_shape& shape, const int x, const int y, const int x_offset, const int y_offset)
	{
		double x_pos = static_cast<double>(x + x_offset);
		double y_pos = static_cast<double>(y + y_offset);

		switch (shape)
		{
			case Rock_shape::HORIZONTAL_LINE:
				m_points = {
					{ x_pos, y_pos },
					{ x_pos + 1, y_pos },
					{ x_pos + 2, y_pos },
					{ x_pos + 3, y_pos },
				};
				break;
			case Rock_shape::PLUS:
				m_points = {
					{ x_pos + 1, y_pos },
					{ x_pos, y_pos + 1 },
					{ x_pos + 1, y_pos + 1 },
					{ x_pos + 2, y_pos + 1 },
					{ x_pos + 1, y_pos + 2 }
				};
				break;
			case Rock_shape::BACKWARDS_L:
				m_points = {
					{ x_pos, y_pos },
					{ x_pos + 1, y_pos },
					{ x_pos + 2, y_pos },
					{ x_pos + 2, y_pos + 1 },
					{ x_pos + 2, y_pos + 2 },
				};
				break;
			case Rock_shape::VERTICAL_LINE:
				m_points = {
					{ x_pos, y_pos },
					{ x_pos, y_pos + 1 },
					{ x_pos, y_pos + 2 },
					{ x_pos, y_pos + 3 }
				};
				break;
			case Rock_shape::BLOCK:
				m_points = {
					{ x_pos, y_pos} ,
					{ x_pos + 1, y_pos },
					{ x_pos, y_pos + 1 },
					{ x_pos + 1, y_pos + 1 }
				};
				break;
			default:
				break;
		}
	}

	bool Rock::try_move(const Rock_move_direction& direction, const std::unordered_set<Point>& rocks, const int min_x, const int max_x, const int min_y)
	{
		Point delta = Point::origin;
		switch (direction)
		{
			case Rock_move_direction::LEFT:
				delta = { -1, 0 };
				break;
			case Rock_move_direction::RIGHT:
				delta = { 1, 0 };
				break;
			case Rock_move_direction::DOWN:
				delta = { 0, -1 };
				break;
			default:
				break;
		}

		for (const Point& p : m_points)
		{
			Point next = p + delta;
			if (next.x < min_x || next.x > max_x || next.y < min_y || rocks.contains(next))
			{
				return false;
			}
		}

		apply_delta(delta);

		return true;
	}

	int64_t Rock::highest_y_value() const
	{
		int64_t highest = std::numeric_limits<int>::min();
		for (const Point& p : m_points)
		{
			highest = std::max(highest, static_cast<int64_t>(p.y));
		}

		return highest;
	}

	void Rock::apply_delta(const Point& delta)
	{
		for (Point& p : m_points)
		{
			p += delta;
		}
	}

	std::ostream& operator<<(std::ostream& os, const Rock& rock)
	{
		for (const Point& p : rock.m_points)
		{
			os << p << ' ';
		}

		return os;
	}

	// ------------------------------------------------------------------------------------
	// Tetris simulator
	// ------------------------------------------------------------------------------------

	Tetris_simulator::Tetris_simulator(const std::vector<Rock_move_direction>& jet_directions, const int min_x, const int max_x, const int min_y, const int offset_x, const int offset_y) :
		m_jets(jet_directions), m_jet_idx(0), m_rock_shape(0), m_height(0), m_min_x(min_x), m_max_x(max_x), m_min_y(min_y), m_offset_x(offset_x), m_offset_y(offset_y)
	{
	}
	
	void Tetris_simulator::drop_rock()
	{
		// Create rock with the next shape
		Rock_shape shape = static_cast<Rock_shape>(m_rock_shape);
		m_rock_shape = (m_rock_shape + 1) % 5;

		Rock rock(shape, m_min_x, m_height, m_offset_x, m_offset_y);

		// Keep dropping rock as long as it can move
		bool can_move_down = true;
		while (can_move_down)
		{
			Rock_move_direction direction = m_jets[m_jet_idx];
			m_jet_idx = (m_jet_idx + 1) % m_jets.size();

			rock.try_move(direction, m_rocks, m_min_x, m_max_x, m_min_y);
			can_move_down = rock.try_move(Rock_move_direction::DOWN, m_rocks, m_min_x, m_max_x, m_min_y);
		}

		// Rock stopped moving, so we need to update the height and add its points to the rock set
		m_height = std::max(m_height, rock.highest_y_value());
		m_rocks.insert(rock.points().begin(), rock.points().end());
	}

	int64_t Tetris_simulator::drop_rocks_with_cycle_detection(const int64_t max_rocks_to_drop)
	{
		std::map<Tetris_state, std::pair<int64_t, int64_t>> states;
		auto cycle_entry = states.end();
		int64_t remaining = max_rocks_to_drop;
		while (remaining > 0)
		{
			drop_rock();
			--remaining;

			// Create key for this state and check if it already exists
			Tetris_state state{ create_height_profile(), m_jet_idx, m_rock_shape};
			const auto cycle_it = states.find(state);
			if (cycle_it != states.end())
			{
				cycle_entry = cycle_it;
				break;
			}

			// Add this state to the state map if it doesn't exist
			states[state] = std::make_pair(remaining, m_height);
		}

		// Return the current height if a cycle wasn't found
		if (cycle_entry == states.end())
			return m_height;

		// Calculate how many iterations this cycle took
		const auto& [_, value] = *cycle_entry;
		int64_t cycle_length = value.first - remaining;
		int64_t height_difference = m_height - value.second;

		// Simulate the rest of the iterations
		int64_t simulated_height = height_difference * (remaining / cycle_length);
		remaining %= cycle_length;

		// Drop the rest of the rocks
		while (remaining > 0)
		{
			drop_rock();
			--remaining;
		}

		return simulated_height + m_height;
	}

	std::array<int64_t, 7> Tetris_simulator::create_height_profile() const
	{
		std::array<int64_t, 7> profile = {};
		for (const Point& p : m_rocks)
		{
			int64_t x = static_cast<int64_t>(p.x);
			int64_t y = static_cast<int64_t>(p.y);

			profile[x] = m_height - std::max(profile[x], y);
		}

		return profile;
	}

} // aoc
