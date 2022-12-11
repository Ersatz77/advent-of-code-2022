#ifndef AOC_2022_DAY_11_MONKEY_H
#define AOC_2022_DAY_11_MONKEY_H

#include <iostream>
#include <vector>
#include <queue>
#include <cstdint>

namespace aoc
{
	enum class Monkey_operand_type
	{
		OLD_VALUE = 0,
		LITERAL
	};

	enum class Monkey_operator
	{
		ADD = 0,
		MULTIPLY
	};

	class Monkey_operation
	{
	public:
		Monkey_operation() = default;
		Monkey_operation(const Monkey_operand_type lhs_type, const Monkey_operand_type rhs_type,
			const uint64_t lhs_literal_value, const uint64_t rhs_literal_value, const Monkey_operator op);

		uint64_t execute(const uint64_t item) const;

		friend std::istream& operator>>(std::istream& is, Monkey_operation& operation);
		friend std::ostream& operator<<(std::ostream& os, const Monkey_operation& operation);

	private:
		Monkey_operand_type m_lhs_type;
		Monkey_operand_type m_rhs_type;
		uint64_t m_lhs_literal_value;
		uint64_t m_rhs_literal_value;
		Monkey_operator m_op;
	};

	class Monkey_test
	{
	public:
		Monkey_test() = default;
		Monkey_test(const uint64_t divisor, const int on_true_id, const int on_false_id);

		int execute(const uint64_t item) const;
		uint64_t get_divisor() const { return m_divisor; };

		friend std::istream& operator>>(std::istream& is, Monkey_test& test);
		friend std::ostream& operator<<(std::ostream& os, const Monkey_test& test);

	private:
		uint64_t m_divisor;
		int m_on_true_id;
		int m_on_false_id;
	};

	class Monkey
	{
	public:
		Monkey() = default;
		Monkey(const int id, const std::vector<uint64_t> items, const Monkey_operation op, const Monkey_test test);

		void add_item(const uint64_t item);
		void inspect_items(std::vector<Monkey>& monkeys, const bool enable_relief);

		int id() const { return m_id; };
		uint64_t inspected_items() const { return m_inspected_items; };
		const Monkey_test& get_test() const { return m_test; };
		const uint64_t get_divisor_product() { return m_divisor_product; };
		void set_divisor_product(const uint64_t divisor_product) { m_divisor_product = divisor_product; };

		friend std::istream& operator>>(std::istream& is, Monkey& monkey);
		friend std::ostream& operator<<(std::ostream& os, const Monkey& monkey);

	private:
		int m_id;

		std::queue<uint64_t> m_items;
		uint64_t m_inspected_items;

		Monkey_operation m_operation;
		Monkey_test m_test;

		uint64_t m_divisor_product;
	};

} // aoc

#endif // !AOC_2022_DAY_11_MONKEY_H
