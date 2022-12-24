#ifndef AOC_2022_DAY_24_VALLEY_H
#define AOC_2022_DAY_24_VALLEY_H

#include "utility/point.h"

#include <vector>
#include <cstdint>

namespace aoc
{
	enum class Blizzard_direction
	{
		NORTH = 0,
		SOUTH,
		EAST,
		WEST
	};

	struct Blizzard
	{
		Point pos;
		Blizzard_direction direction;
	};

	class Valley
	{
	public:
		Valley(const std::vector<Blizzard>& blizzards);

		int64_t shortest_time_from(const Point& start, const Point& end);
		Point start() const { return m_start; };
		Point end() const { return m_end; }

	private:
		bool in_bounds(const Point& p) const;
		bool has_blizzard(const Point& p) const;
		bool can_move_to(const Point& p, const Point& start, const Point& end) const;
		void update_blizzards();

		std::vector<Blizzard> m_blizzards;
		int64_t m_blizzard_time;

		Point m_start;
		Point m_end;

		int64_t m_min_x;
		int64_t m_max_x;
		int64_t m_min_y;
		int64_t m_max_y;
	};

} // aoc

#endif // !AOC_2022_DAY_24_VALLEY_H
