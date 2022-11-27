#include "utility/point.h"

#include "fmt/format.h"

#include <iostream>
#include <string>
#include <array>

namespace aoc
{
	// --------------------------------------------------------------------------------
	// Point
	// --------------------------------------------------------------------------------

	const Point Point::origin(0.0, 0.0);

	// Returns the 4 (NESW) points around this point
	std::array<Point, 4> Point::adjacent_cardinal() const
	{
		std::array<Point, 4> adjacent = {};

		constexpr std::array<std::array<double, 2>, 4> offsets = { {{0, 1}, {0, -1}, {1, 0}, {-1, 0}} };
		for (size_t i = 0; i < offsets.size(); ++i)
		{
			adjacent[i].x = x + offsets[i][0];
			adjacent[i].y = y + offsets[i][1];
		}

		return adjacent;
	}

	// Returns all 8 points around this point
	std::array<Point, 8> Point::adjacent() const
	{
		std::array<Point, 8> adjacent = {};

		constexpr std::array<std::array<double, 2>, 8> offsets = { {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}} };
		for (size_t i = 0; i < offsets.size(); ++i)
		{
			adjacent[i].x = x + offsets[i][0];
			adjacent[i].y = y + offsets[i][1];
		}

		return adjacent;
	}

	std::string Point::to_string() const
	{
		return fmt::format("({},{})", x, y);
	}

	std::array<double, 2> Point::to_array() const
	{
		return { x, y };
	}

	Point& Point::operator+=(const Point& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Point& Point::operator-=(const Point& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Point& Point::operator*=(const Point& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Point& Point::operator/=(const Point& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	Point operator+(const Point& lhs, const Point& rhs)
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y };
	}

	Point operator-(const Point& lhs, const Point& rhs)
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y };
	}

	Point operator*(const Point& lhs, const Point& rhs)
	{
		return { lhs.x * rhs.x, lhs.y * rhs.y };
	}

	Point operator/(const Point& lhs, const Point& rhs)
	{
		return { lhs.x / rhs.x, lhs.y / rhs.y };
	}

	std::ostream& operator<<(std::ostream& os, const Point& p)
	{
		return os << p.to_string();
	}

	// --------------------------------------------------------------------------------
	// Point_3d
	// --------------------------------------------------------------------------------

	const Point_3d Point_3d::origin{ 0.0, 0.0, 0.0 };

	std::string Point_3d::to_string() const
	{
		return fmt::format("({},{},{})", x, y, z);
	}

	std::array<double, 3> Point_3d::to_array() const
	{
		return { x, y, z };
	}

	Point_3d& Point_3d::operator+=(const Point_3d& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Point_3d& Point_3d::operator-=(const Point_3d& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Point_3d& Point_3d::operator*=(const Point_3d& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Point_3d& Point_3d::operator/=(const Point_3d& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	Point_3d operator+(const Point_3d& lhs, const Point_3d& rhs)
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
	}

	Point_3d operator-(const Point_3d& lhs, const Point_3d& rhs)
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
	}

	Point_3d operator*(const Point_3d& lhs, const Point_3d& rhs)
	{
		return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
	}

	Point_3d operator/(const Point_3d& lhs, const Point_3d& rhs)
	{
		return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
	}

	std::ostream& operator<<(std::ostream& os, const Point_3d& p)
	{
		return os << p.to_string();
	}

} // aoc
