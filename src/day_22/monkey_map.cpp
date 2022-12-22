#include "day_22/monkey_map.h"

#include "utility/point.h"
#include "utility/math.h"

#include <array>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <utility>

namespace aoc
{
	Monkey_map::Monkey_map(const std::unordered_map<Point, char>& grid) :
		m_grid(grid), m_width(0), m_height(0), m_side_length(0), m_pos({1, 1}), m_facing(0)
	{
		for (const auto& [p, _] : m_grid)
		{
			m_width = std::max(m_width, static_cast<int>(p.x));
			m_height = std::max(m_height, static_cast<int>(p.y));
		}

		while (!m_grid.contains(m_pos))
		{
			m_pos += {1, 0};
		}

		m_side_length = std::gcd(m_width, m_height);
	}

	void Monkey_map::walk(const Movement_instruction& instruction, const bool is_3d)
	{
		// Handle rotation instructions
		switch (instruction.type)
		{
			case Movement_type::TURN_LEFT:
				m_facing = (m_facing + 3) % 4;
				return;
			case Movement_type::TURN_RIGHT:
				m_facing = (m_facing + 1) % 4;
				return;
			default:
				break;
		}

		// Handle move instructions
		constexpr std::array<Point, 4> deltas = { {{1, 0}, {0, 1}, {-1, 0}, {0, -1}} };
		int step = 0;
		while (step < instruction.amount)
		{
			auto [next, next_facing] = is_3d ? get_next_3d(m_pos, deltas[m_facing]) : get_next(m_pos, deltas[m_facing]);
			if (m_grid.at(next) == '#')
				break;

			m_pos = next;
			m_facing = next_facing;
			++step;
		}
	}

	int Monkey_map::password() const
	{
		return (1000 * m_pos.y) + (4 * m_pos.x) + m_facing;
	}

	std::pair<Point, int> Monkey_map::get_next(const Point& p, const Point& delta) const
	{
		Point next = p;
		do
		{
			next += delta;
			next.x = euclid_mod(static_cast<int>(next.x), m_width + 1);
			next.y = euclid_mod(static_cast<int>(next.y), m_height + 1);

		} while (!m_grid.contains(next));

		return std::make_pair(next, m_facing);
	}

	std::pair<Point, int> Monkey_map::get_next_3d(const Point& p, const Point& delta) const
	{
		Point next = p + delta;
		int next_facing = m_facing;

        // This is a hardcoded mess that doesn't work for the example
        // Basicaly it just "teleports" the new position around the cube
        if (m_facing == 0)
        {
            if (next.x == (m_side_length * 3) + 1)
            {
                // 5 ->  6
                next.y = ((m_side_length + 1) - next.y) + (m_side_length * 2);
                next.x = m_side_length * 2;
                next_facing = 2;
            }
            else if (next.x == (m_side_length * 2) + 1 && next.y >= m_side_length + 1 && next.y <= m_side_length * 2)
            {
                // 4 -> 5
                next.x = next.y + m_side_length;
                next.y = m_side_length;
                next_facing = 3;
            }
            else if (next.x == (m_side_length * 2) + 1 && next.y >= (m_side_length * 2) + 1)
            {
                // 6 -> 5
                next.y = (m_side_length + 1) - (next.y - (m_side_length * 2));
                next.x = m_side_length * 3;
                next_facing = 2;
            }
            else if (next.x == m_side_length + 1 && next.y >= (m_side_length * 3) + 1)
            {
                // 3 -> 6
                next.x = next.y - (m_side_length * 2);
                next.y = m_side_length * 3;
                next_facing = 3;
            }
        }
        else if (m_facing == 1)
        {
            if (next.y == (m_side_length * 4) + 1)
            {
                // 3 -> 5
                next.x = next.x + (m_side_length * 2);
                next.y = 1;
                next_facing = 1;
            }
            else if (next.y == (m_side_length * 3) + 1 && next.x >= m_side_length + 1)
            {
                // 6 -> 3
                next.y = next.x + (m_side_length * 2);
                next.x = m_side_length;
                next_facing = 2;
            }
            else if (next.y == m_side_length + 1 && next.x >= (m_side_length * 2) + 1)
            {
                // 5 -> 4
                next.y = next.x - m_side_length;
                next.x = m_side_length * 2;
                next_facing = 2;
            }
        }
        else if (m_facing == 2)
        {
            if (next.x == m_side_length && next.y <= m_side_length)
            {
                // 1 -> 2
                next.y = ((m_side_length + 1) - next.y) + (m_side_length * 2);
                next.x = 1;
                next_facing = 0;
            }
            else if (next.x == m_side_length && next.y >= m_side_length + 1 && next.y <= m_side_length * 2)
            {
                // 4 -> 2
                next.x = next.y - m_side_length;
                next.y = (m_side_length * 2) + 1;
                next_facing = 1;
            }
            else if (next.x == 0 && next.y <= m_side_length * 3)
            {
                // 2 -> 1
                next.y = (m_side_length + 1) - (next.y - (m_side_length * 2));
                next.x = m_side_length + 1;
                next_facing = 0;
            }
            else if (next.x == 0 && next.y >= (m_side_length * 3) + 1)
            {
                // 3 -> 1
                next.x = next.y - (m_side_length * 2);
                next.y = 1;
                next_facing = 1;
            }
        }
        else if (m_facing == 3)
        {
            if (next.y == m_side_length * 2 && next.x <= m_side_length)
            {
                // 2 -> 4
                next.y = next.x + m_side_length;
                next.x = m_side_length + 1;
                next_facing = 0;
            }
            else if (next.y == 0 && next.x <= m_side_length * 2)
            {
                // 1 -> 3
                next.y = next.x + (m_side_length * 2);
                next.x = 1;
                next_facing = 0;
            }
            else if (next.y == 0 && next.x >= (m_side_length * 2) + 1)
            {
                // 5 -> 3
                next.x = next.x - (m_side_length * 2);
                next.y = m_side_length * 4;
                next_facing = 3;
            }
        }

		return std::make_pair(next, next_facing);
	}

} // aoc
