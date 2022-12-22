#ifndef AOC_2022_DAY_22_MONKEY_MAP_H
#define AOC_2022_DAY_22_MONKEY_MAP_H

#include "utility/point.h"

#include <unordered_map>
#include <utility>

namespace aoc
{
	enum class Movement_type
	{
		MOVE = 0,
		TURN_RIGHT,
		TURN_LEFT
	};

	struct Movement_instruction
	{
		Movement_type type;
		int amount = 0;
	};

	class Monkey_map
	{
	public:
		Monkey_map(const std::unordered_map<Point, char>& grid);

		void walk(const Movement_instruction& instruction, const bool is_3d = false);
		int password() const;

	private:
		std::pair<Point, int> get_next(const Point& p, const Point& delta) const;
		std::pair<Point, int> get_next_3d(const Point& p, const Point& delta) const;

		std::unordered_map<Point, char> m_grid;
		int m_width;
		int m_height;
		int m_side_length;

		Point m_pos;
		int m_facing;
	};

} // aoc

#endif // !AOC_2022_DAY_22_MONKEY_MAP_H
