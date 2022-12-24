#include "day_24/valley.h"

#include "utility/point.h"

#include <vector>
#include <array>
#include <unordered_set>
#include <algorithm>
#include <limits>
#include <cstdint>

namespace aoc
{
	Valley::Valley(const std::vector<Blizzard>& blizzards) :
		m_blizzards(blizzards),
		m_blizzard_time(0),
		m_min_x(std::numeric_limits<int64_t>::max()),
		m_max_x(std::numeric_limits<int64_t>::min()),
		m_min_y(std::numeric_limits<int64_t>::max()),
		m_max_y(std::numeric_limits<int64_t>::min())
	{
		for (const Blizzard& b : m_blizzards)
		{
			m_min_x = std::min(m_min_x, static_cast<int64_t>(b.pos.x));
			m_max_x = std::max(m_max_x, static_cast<int64_t>(b.pos.x));
			m_min_y = std::min(m_min_y, static_cast<int64_t>(b.pos.y));
			m_max_y = std::max(m_max_y, static_cast<int64_t>(b.pos.y));
		}

		m_start = Point(m_min_x, m_min_y - 1);
		m_end = Point(m_max_x, m_max_y + 1);
	}

	int64_t Valley::shortest_time_from(const Point& start, const Point& end)
	{
		// This can definitely be optimized

		std::unordered_set<Point> to_visit({ start });
		int64_t minutes = 0;
		while (!to_visit.contains(end))
		{
			update_blizzards();

			std::unordered_set<Point> next_to_visit;
			for (const auto& current : to_visit)
			{
				// Try moving in each cardinal direction
				for (const Point& adj : current.adjacent_cardinal())
				{
					if (can_move_to(adj, start, end))
					{
						next_to_visit.insert(adj);
					}
				}

				// Try waiting at this position
				if (can_move_to(current, start, end))
				{
					next_to_visit.insert(current);
				}
			}

			to_visit = next_to_visit;
			++minutes;
		}

		return minutes;
	}

	bool Valley::in_bounds(const Point& p) const
	{
		return (p.x >= m_min_x && p.x <= m_max_x && p.y >= m_min_y && p.y <= m_max_y);
	}

	bool Valley::has_blizzard(const Point& p) const
	{
		for (const Blizzard& b : m_blizzards)
		{
			if (p == b.pos)
			{
				return true;
			}
		}

		return false;
	}

	bool Valley::can_move_to(const Point& p, const Point& start, const Point& end) const
	{
		return (in_bounds(p) || p == start || p == end) && !has_blizzard(p);
	}

	void Valley::update_blizzards()
	{
		constexpr std::array<Point, 4> deltas = { {{0, -1}, {0, 1}, {1, 0}, {-1, 0}} };
		for (Blizzard& b : m_blizzards)
		{
			b.pos += deltas[static_cast<size_t>(b.direction)];
			switch (b.direction)
			{
				case Blizzard_direction::NORTH:
					b.pos.y = (b.pos.y < m_min_y) ? m_max_y : b.pos.y;
					break;
				case Blizzard_direction::SOUTH:
					b.pos.y = (b.pos.y > m_max_y) ? m_min_y : b.pos.y;
					break;
				case Blizzard_direction::EAST:
					b.pos.x = (b.pos.x > m_max_x) ? m_min_x : b.pos.x;
					break;
				case Blizzard_direction::WEST:
					b.pos.x = (b.pos.x < m_min_x) ? m_max_x : b.pos.x;
					break;
				default:
					break;
			}
		}
	}

} // aoc
