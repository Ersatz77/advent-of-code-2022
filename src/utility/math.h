#ifndef AOC_UTILITIES_MATH_H
#define AOC_UTILITIES_MATH_H

#include "utility/point.h"

#include <vector>
#include <cmath>
#include <type_traits>

namespace aoc
{
	template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
	T manhattan_distance(const T x1, const T x2, const T y1, const T y2)
	{
		return std::abs(x1 - x2) + std::abs(y1 - y2);
	}

	template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
	T manhattan_distance(const T x1, const T x2, const T y1, const T y2, const T z1, const T z2)
	{
		return std::abs(x1 - x2) + std::abs(y1 - y2) + std::abs(z1 - z2);
	}

	double manhattan_distance(const Point& p1, const Point& p2);
	double manhattan_distance(const Point_3d& p1, const Point_3d& p2);

	// Modular multiplicative inverse
	template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
	T mod_mul_inv(T a, T b)
	{
		if (b == 1)
		{
			return 1;
		}

		T b0 = b;
		T t = 0;
		T q = 0;
		T x0 = 0;
		T x1 = 1;

		while (a > 1)
		{
			q = a / b;
			t = b;
			b = a % b;
			a = t;
			t = x0;
			x0 = x1 - q * x0;
			x1 = t;
		}

		if (x1 < 0)
		{
			x1 += b0;
		}

		return x1;
	}

	// Chinese remainder theorem
	template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
	T chinese_remainder(const std::vector<T>& n, const std::vector<T>& a)
	{
		T product = 1;
		for (const T& i : n)
		{
			product *= i;
		}

		T sum = 0;
		for (size_t i = 0; i < n.size(); i++)
		{
			T p = product / n[i];
			sum += a[i] * mod_mul_inv(p, n[i]) * p;
		}

		return sum % product;
	}

	template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
	T pow(T base, T exp)
	{
		T result = 1;
		while (exp)
		{
			if (exp % 2)
			{
				result *= base;
			}

			exp /= 2;
			base *= base;
		}

		return result;
	}

} // aoc

#endif // !AOC_UTILITIES_MATH_H
