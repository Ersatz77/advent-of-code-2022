#ifndef AOC_2022_DAY_13_LIST_H
#define AOC_2022_DAY_13_LIST_H

#include <iostream>
#include <variant>
#include <vector>
#include <compare>

namespace aoc
{
	struct List : public std::variant<std::vector<List>, int>
	{
		// Bring constructors into this scope
		using std::variant<std::vector<List>, int>::variant;

		// Utility function to get this list as a vector of lists
		std::vector<List>& as_list() { return std::get<std::vector<List>>(*this); }

		// Utility function to get this list as an int
		int& as_int() { return std::get<int>(*this); }

		// Compares two lists together
		std::weak_ordering compare(List& other);

		List* parent = nullptr;
	};

	void parse_packet(std::istream& is, List& list);
	std::ostream& operator<<(std::ostream& os, List& list);

} // aoc

#endif // !AOC_2022_DAY_13_LIST_H
