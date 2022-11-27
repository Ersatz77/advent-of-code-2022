#ifndef AOC_UTILITIES_DAY_H
#define AOC_UTILITIES_DAY_H

#include <filesystem>
#include <string>

namespace aoc
{
	// An abstract class for a day
	class Day
	{
	public:
		Day() = default;
		virtual ~Day() = default;
		virtual std::string part_1(const std::filesystem::path& input_path) const = 0;
		virtual std::string part_2(const std::filesystem::path& input_path) const = 0;
	};

} // aoc

#endif // !AOC_UTILITIES_DAY_H
