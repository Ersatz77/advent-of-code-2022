#include "day_9/day_9.h"

#include "utility/io.h"
#include "utility/point.h"
#include "utility/math.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>

namespace aoc
{
	enum class Direction
	{
		LEFT = 0,
		RIGHT,
		UP,
		DOWN,
		UNKNOWN
	};

	struct Instruction
	{
		Direction direction;
		int amount;
	};

	static std::vector<Instruction> parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		std::vector<Instruction> instructions;
		char dir = ' ';
		int amount = 0;
		while (file >> dir >> amount)
		{
			switch (std::toupper(dir))
			{
				case 'L':
					instructions.emplace_back(Direction::LEFT, amount);
					break;
				case 'R':
					instructions.emplace_back(Direction::RIGHT, amount);
					break;
				case 'U':
					instructions.emplace_back(Direction::UP, amount);
					break;
				case 'D':
					instructions.emplace_back(Direction::DOWN, amount);
					break;
				default:
					instructions.emplace_back(Direction::UNKNOWN, -1);
					break;
			}
		}

		return instructions;
	}

	static bool is_tail_adjacent_to_head(const Point& head, const Point& tail)
	{
		for (const auto& adj : head.adjacent())
		{
			if (adj == tail)
			{
				return true;
			}
		}

		return false;
	}

	static void move_rope_head(const Direction& direction, Point& head)
	{
		switch (direction)
		{
			case Direction::LEFT:
				--head.x;
				break;
			case Direction::RIGHT:
				++head.x;
				break;
			case Direction::UP:
				++head.y;
				break;
			case Direction::DOWN:
				--head.y;
				break;
			default:
				break;
		}
	}

	static void move_rope_tail(Point& head, Point& tail)
	{
		// Move tail if it's not adjacent to the head
		if (!is_tail_adjacent_to_head(head, tail))
		{
			tail.x += signum(head.x - tail.x);
			tail.y += signum(head.y - tail.y);
		}
	}

	std::string Day_9::part_1(const std::filesystem::path& input_path) const
	{
		std::vector<Instruction> instructions = parse_input(input_path / "day_9.txt");
		
		std::unordered_set<Point> unique_tail_points;
		Point head(0, 0);
		Point tail(0, 0);
		for (const auto& instruction : instructions)
		{
			for (int i = 0; i < instruction.amount; ++i)
			{
				move_rope_head(instruction.direction, head);
				move_rope_tail(head, tail);
				unique_tail_points.insert(tail);
			}
		}

		return fmt::format("Day 9 Part 1 | Unique positions the rope's tail visited: {}", unique_tail_points.size());
	}

	std::string Day_9::part_2(const std::filesystem::path& input_path) const
	{
		std::vector<Instruction> instructions = parse_input(input_path / "day_9.txt");

		std::unordered_set<Point> unique_tail_points;
		std::vector<Point> rope(10);
		for (const auto& instruction : instructions)
		{
			for (int i = 0; i < instruction.amount; ++i)
			{
				move_rope_head(instruction.direction, rope.front());

				for (size_t j = 1; j < rope.size(); ++j)
				{
					move_rope_tail(rope[j - 1], rope[j]);
				}

				unique_tail_points.insert(rope.back());
			}
		}

		return fmt::format("Day 9 Part 2 | Unique positions the long rope's tail visited: {}", unique_tail_points.size());
	}

} // aoc
