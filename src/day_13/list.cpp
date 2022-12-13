#include "day_13/list.h"

#include <iostream>
#include <variant>
#include <vector>
#include <algorithm>
#include <compare>

namespace aoc
{
	std::weak_ordering List::compare(List& other)
	{
		// If both lists are ints, compare using the spaceship operator
		if (index() == 1 && other.index() == 1)
			return as_int() <=> other.as_int();

		// Get both values as lists
		std::vector<List> this_list;
		std::vector<List> other_list;

		// If left is an int, convert it to a list
		// Otherwise, extract it as a list
		if (index() == 1)
			this_list.push_back(as_int());
		else
			this_list = as_list();

		// If right is an int, convert it to a list
		// Otherwise, extract it as a list
		if (other.index() == 1)
			other_list.push_back(other.as_int());
		else
			other_list = other.as_list();

		// Compare list elements and skip the rest of the elements if the lists aren't the same size
		size_t min_size = std::min(this_list.size(), other_list.size());
		for (size_t i = 0; i < min_size; ++i)
		{
			const auto result = this_list[i].compare(other_list[i]);
			if (result != std::weak_ordering::equivalent)
				return result;
		}

		// Compare the size of the lists using the spaceship operator
		return this_list.size() <=> other_list.size();
	}

	static void parse_comma(std::istream& is)
	{
		if (is.peek() == ',')
		{
			is.ignore(1);
		}
	}

	void parse_packet(std::istream& is, List& list)
	{
		// Extract the list
		auto& current = list.as_list();

		// Get the first character
		char c = ' ';
		is >> c;

		if (c == '[')
		{
			// Create a new list
			List next = std::vector<List>{};
			next.parent = &list;
			current.emplace_back(next);

			// Parse next packet
			parse_packet(is, current.back());
		}
		else if (c == ']')
		{
			// Keep parsing the packet if we aren't in the outermost list
			if (list.parent)
			{
				parse_comma(is);
				parse_packet(is, *list.parent);
			}
		}
		else
		{
			is.putback(c);

			int value = 0;
			is >> value;
			current.push_back(value);

			parse_comma(is);
			parse_packet(is, list);
		}
	}

	std::ostream& operator<<(std::ostream& os, List& list)
	{
		if (list.index() == 0)
		{
			os << '[';
			for (auto& val : list.as_list())
			{
				os << val << ',';
			}

			os << ']';
		}
		else
		{
			os << list.as_int();
		}

		return os;
	}

} // aoc
