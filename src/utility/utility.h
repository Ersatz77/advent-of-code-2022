#ifndef AOC_UTILITIES_UTILITY_H
#define AOC_UTILITIES_UTILITY_H

#include <vector>
#include <array>
#include <utility>
#include <cstdint>
#include <type_traits>

namespace aoc
{
	template<class T>
	inline void hash_combine(size_t& seed, const T& v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	// Creates an integer number out of a vector of digits
	template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
	T vec_to_int(const std::vector<T>& vec)
	{
		T result = 0;
		for (const T i : vec)
		{
			result *= 10;
			result += i;
		}

		return result;
	}

	// Creates an integer number out of an array of digits
	template<typename T, size_t N, typename = std::enable_if_t<std::is_integral_v<T>>>
	T array_to_int(const std::array<T, N>& arr)
	{
		T result = 0;
		for (const T i : arr)
		{
			result *= 10;
			result += i;
		}

		return result;
	}

	template<size_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
	std::array<T, N> int_to_aray(T val)
	{
		std::array<T, N> digits = {};
		for (size_t i = digits.size(); i > 0; --i)
		{
			digits[i - 1] = val % 10;
			val /= 10;
		}

		return digits;
	}

} // aoc

namespace std
{
	template <>
	struct hash<pair<char, char>>
	{
		std::size_t operator()(const pair<char, char>& p) const
		{
			size_t seed = 0;
			aoc::hash_combine(seed, p.first);
			aoc::hash_combine(seed, p.second);
			return seed;
		}
	};

	template <>
	struct hash<pair<int, int>>
	{
		std::size_t operator()(const pair<int, int>& p) const
		{
			size_t seed = 0;
			aoc::hash_combine(seed, p.first);
			aoc::hash_combine(seed, p.second);
			return seed;
		}
	};

} // std

#endif // !AOC_UTILITIES_UTILITY_H
