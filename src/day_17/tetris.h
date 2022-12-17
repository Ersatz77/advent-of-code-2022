#ifndef AOC_2022_DAY_17_TETRIS_H
#define AOC_2022_DAY_17_TETRIS_H

#include "utility/point.h"

#include <iostream>
#include <vector>
#include <array>
#include <unordered_set>
#include <cstdint>

namespace aoc
{
	enum class Rock_move_direction
	{
		LEFT = 0,
		RIGHT,
		DOWN,
		UNKNOWN
	};

	enum class Rock_shape
	{
		HORIZONTAL_LINE = 0,
		PLUS,
		BACKWARDS_L,
		VERTICAL_LINE,
		BLOCK
	};

	class Rock
	{
	public:
		Rock() = default;
		Rock(const Rock_shape& shape, const int x, const int y, const int x_offset, const int y_offset);

		bool try_move(const Rock_move_direction& direction, const std::unordered_set<Point>& rocks, const int min_x, const int max_x, const int min_y);
		int64_t highest_y_value() const;

		const std::vector<Point>& points() const { return m_points; }
		std::vector<Point>& points() { return m_points; }

		friend std::ostream& operator<<(std::ostream& os, const Rock& rock);

	private:
		void apply_delta(const Point& delta);

		std::vector<Point> m_points;
	};

	class Tetris_simulator
	{
	public:
		Tetris_simulator() = default;
		Tetris_simulator(const std::vector<Rock_move_direction>& jet_directions, const int min_x = 0, const int max_x = 6, const int min_y = 1, const int offset_x = 2, const int offset_y = 4);

		void drop_rock();
		int64_t drop_rocks_with_cycle_detection(const int64_t max_rocks_to_drop);
		int64_t height() const { return m_height; };
		
	private:
		std::array<int64_t, 7> create_height_profile() const;

		std::vector<Rock_move_direction> m_jets;
		size_t m_jet_idx;
		int m_rock_shape;
		int64_t m_height;

		int m_min_x;
		int m_max_x;
		int m_min_y;
		int m_offset_x;
		int m_offset_y;

		std::unordered_set<Point> m_rocks;
	};

} // aoc

#endif // !AOC_2022_DAY_17_TETRIS_H
