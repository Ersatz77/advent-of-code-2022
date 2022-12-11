#include "day_11/monkey.h"

#include "utility/string.h"

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdint>

namespace aoc
{
	// ------------------------------------------------------------------------------------
	// Monkey operation
	// ------------------------------------------------------------------------------------

	Monkey_operation::Monkey_operation(const Monkey_operand_type lhs_type, const Monkey_operand_type rhs_type,
		const uint64_t lhs_literal_value, const uint64_t rhs_literal_value, const Monkey_operator op) :
		m_lhs_type(lhs_type), m_rhs_type(rhs_type),
		m_lhs_literal_value(lhs_literal_value), m_rhs_literal_value(rhs_literal_value), m_op(op)
	{
	}

	uint64_t Monkey_operation::execute(const uint64_t item) const
	{
		uint64_t lhs = (m_lhs_type == Monkey_operand_type::OLD_VALUE) ? item : m_lhs_literal_value;
		uint64_t rhs = (m_rhs_type == Monkey_operand_type::OLD_VALUE) ? item : m_rhs_literal_value;

		return (m_op == Monkey_operator::ADD) ? lhs + rhs : lhs * rhs;
	}

	std::istream& operator>>(std::istream& is, Monkey_operation& operation)
	{
		std::string line;

		std::getline(is, line);
		std::vector<std::string> operation_parts = split(regex_split(regex_split(line, ": ").back(), "= ").back(), ' ');

		operation.m_lhs_type = (operation_parts[0] == "old") ? Monkey_operand_type::OLD_VALUE : Monkey_operand_type::LITERAL;
		operation.m_rhs_type = (operation_parts[2] == "old") ? Monkey_operand_type::OLD_VALUE : Monkey_operand_type::LITERAL;
		operation.m_lhs_literal_value = (operation_parts[0] != "old") ? std::stoi(operation_parts[0]) : -1;
		operation.m_rhs_literal_value = (operation_parts[2] != "old") ? std::stoi(operation_parts[2]) : -1;
		operation.m_op = (operation_parts[1] == "+") ? Monkey_operator::ADD : Monkey_operator::MULTIPLY;

		return is;
	}

	std::ostream& operator<<(std::ostream& os, const Monkey_operation& operation)
	{
		std::string lhs = (operation.m_lhs_type == Monkey_operand_type::OLD_VALUE) ? "old" : std::to_string(operation.m_lhs_literal_value);
		std::string rhs = (operation.m_rhs_type == Monkey_operand_type::OLD_VALUE) ? "old" : std::to_string(operation.m_rhs_literal_value);
		std::string op = (operation.m_op == Monkey_operator::ADD) ? "+" : "*";

		return os << lhs << ' ' << op << ' ' << rhs;
	}

	// ------------------------------------------------------------------------------------
	// Monkey test
	// ------------------------------------------------------------------------------------

	Monkey_test::Monkey_test(const uint64_t divisor, const int on_true_id, const int on_false_id) :
		m_divisor(divisor), m_on_true_id(on_true_id), m_on_false_id(on_false_id)
	{
	}

	int Monkey_test::execute(const uint64_t item) const
	{
		return (item % m_divisor == 0) ? m_on_true_id : m_on_false_id;
	}

	std::istream& operator>>(std::istream& is, Monkey_test& test)
	{
		std::string line;

		std::getline(is, line);
		test.m_divisor = std::stoi(split(line, ' ').back());

		std::getline(is, line);
		test.m_on_true_id = std::stoi(split(line, ' ').back());

		std::getline(is, line);
		test.m_on_false_id = std::stoi(split(line, ' ').back());

		return is;
	}

	std::ostream& operator<<(std::ostream& os, const Monkey_test& test)
	{
		return os << "Divisor: " << test.m_divisor
			<< " | On true ID: " << test.m_on_true_id
			<< " | On false ID: " << test.m_on_false_id;
	}

	// ------------------------------------------------------------------------------------
	// Monkey
	// ------------------------------------------------------------------------------------

	Monkey::Monkey(const int id, const std::vector<uint64_t> items, const Monkey_operation op, const Monkey_test test) :
		m_id(id), m_inspected_items(0), m_operation(op), m_test(test), m_divisor_product(0)
	{
		for (const uint64_t i : items)
		{
			m_items.push(i);
		}
	}

	void Monkey::add_item(const uint64_t item)
	{
		m_items.push(item);
	}

	void Monkey::inspect_items(std::vector<Monkey>& monkeys, const bool enable_relief)
	{
		while (!m_items.empty())
		{
			uint64_t item = m_items.front();
			m_items.pop();
			++m_inspected_items;

			// Get new worry level
			uint64_t worry_level = 0;
			if (enable_relief)
				worry_level = m_operation.execute(item) / 3;
			else
				worry_level = m_operation.execute(item % m_divisor_product);

			// Get the ID of the monkey to give this item to
			int throw_to_id = m_test.execute(worry_level);

			// Give this item to the next monkey
			const auto to_monkey = std::find_if(monkeys.begin(), monkeys.end(),
				[&throw_to_id](const Monkey& m) { return m.id() == throw_to_id; });
			if (to_monkey != monkeys.end())
			{
				to_monkey->add_item(worry_level);
			}
		}
	}

	std::istream& operator>>(std::istream& is, Monkey& monkey)
	{
		std::string line;

		// Get ID
		std::getline(is, line);
		monkey.m_id = std::stoi(line.substr(7, 1));

		// Get items
		std::getline(is, line);
		for (const std::string& raw_item : split(regex_split(line, ": ").back(), ','))
		{
			monkey.m_items.push(std::stoi(raw_item));
		}

		monkey.m_inspected_items = 0;

		// Get operation
		Monkey_operation operation;
		is >> operation;
		monkey.m_operation = operation;

		// Get test
		Monkey_test test;
		is >> test;
		monkey.m_test = test;

		// Get the empty line
		std::getline(is, line);

		return is;
	}

	std::ostream& operator<<(std::ostream& os, const Monkey& monkey)
	{
		os << "ID: " << monkey.m_id << "\n\tItems: ";
		std::queue<uint64_t> items = monkey.m_items;
		while (!items.empty())
		{
			const uint64_t item = items.front();
			items.pop();

			os << item << ' ';
		}

		return os << "\n\t" << monkey.m_operation
			<< "\n\t" << monkey.m_test;
	}

} // aoc
