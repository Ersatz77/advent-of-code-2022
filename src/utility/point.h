#ifndef AOC_UTILITIES_POINT_H
#define AOC_UTILITIES_POINT_H

#include "utility/utility.h"

#include <iostream>
#include <string>
#include <array>
#include <compare>

namespace aoc
{
	class Point
	{
	public:
		// Returns the 4 (NESW) points around this point
		std::array<Point, 4> adjacent_cardinal() const;

		// Returns all 8 points around this point
		std::array<Point, 8> adjacent() const;

		std::string to_string() const;
		std::array<double, 2> to_array() const;

		auto operator<=>(const Point& other) const = default;
		Point& operator+=(const Point& other);
		Point& operator-=(const Point& other);
		Point& operator*=(const Point& other);
		Point& operator/=(const Point& other);

		double x;
		double y;

		static const Point origin;
	};

	Point operator+(const Point& lhs, const Point& rhs);
	Point operator-(const Point& lhs, const Point& rhs);
	Point operator*(const Point& lhs, const Point& rhs);
	Point operator/(const Point& lhs, const Point& rhs);
	std::ostream& operator<<(std::ostream& os, const Point& p);

	class Point_3d
	{
	public:
		std::string to_string() const;
		std::array<double, 3> to_array() const;

		auto operator<=>(const Point_3d& other) const = default;
		Point_3d& operator+=(const Point_3d& other);
		Point_3d& operator-=(const Point_3d& other);
		Point_3d& operator*=(const Point_3d& other);
		Point_3d& operator/=(const Point_3d& other);

		double x;
		double y;
		double z;

		static const Point_3d origin;
	};

	Point_3d operator+(const Point_3d& lhs, const Point_3d& rhs);
	Point_3d operator-(const Point_3d& lhs, const Point_3d& rhs);
	Point_3d operator*(const Point_3d& lhs, const Point_3d& rhs);
	Point_3d operator/(const Point_3d& lhs, const Point_3d& rhs);
	std::ostream& operator<<(std::ostream& os, const Point_3d& p);

} // aoc

namespace std
{
	template <>
	struct hash<aoc::Point>
	{
		std::size_t operator()(const aoc::Point& p) const
		{
			size_t seed = 0;
			aoc::hash_combine(seed, p.x);
			aoc::hash_combine(seed, p.y);
			return seed;
		}
	};

	template <>
	struct hash<aoc::Point_3d>
	{
		std::size_t operator()(const aoc::Point_3d& p) const
		{
			size_t seed = 0;
			aoc::hash_combine(seed, p.x);
			aoc::hash_combine(seed, p.y);
			aoc::hash_combine(seed, p.z);
			return seed;
		}
	};

} // std

#endif // !AOC_UTILITIES_POINT_H
