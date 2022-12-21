#include "day_21/day_21.h"

#include "utility/io.h"

#include "fmt/format.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <variant>
#include <unordered_map>
#include <cstdint>

namespace aoc
{
	enum class Monkey_job_operator
	{
		UNKNOWN = 0,
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE
	};

	struct Monkey_job
	{
		std::string lhs;
		std::string rhs;
		Monkey_job_operator op = Monkey_job_operator::UNKNOWN;
	};

	using Monkey_map = std::unordered_map<std::string, std::variant<Monkey_job, int64_t>>;

	static Monkey_map parse_input(const std::filesystem::path& path)
	{
		std::ifstream file = open_file(path);

		Monkey_map monkeys;
		for (std::string line; std::getline(file, line) && !line.empty(); )
		{
			std::string key = line.substr(0, 4);

			if (std::isdigit(line[6]))
			{
				int64_t value = std::stoll(line.substr(6));
				monkeys[key] = value;
			}
			else
			{
				std::string lhs = line.substr(6, 4);
				std::string rhs = line.substr(13, 4);
				Monkey_job_operator op = Monkey_job_operator::UNKNOWN;
				switch (line[11])
				{
					case '+':
						op = Monkey_job_operator::ADD;
						break;
					case '-':
						op = Monkey_job_operator::SUBTRACT;
						break;
					case '*':
						op = Monkey_job_operator::MULTIPLY;
						break;
					case '/':
						op = Monkey_job_operator::DIVIDE;
						break;
				}

				monkeys[key] = Monkey_job{ lhs, rhs, op };
			}
		}

		return monkeys;
	}

	static int64_t solve_expression(const Monkey_map& monkeys, const std::string& key)
	{
		const auto& value = monkeys.at(key);
		if (value.index() == 0)
		{
			const Monkey_job& job = std::get<Monkey_job>(value);
			int64_t lhs = solve_expression(monkeys, job.lhs);
			int64_t rhs = solve_expression(monkeys, job.rhs);

			switch (job.op)
			{
				case Monkey_job_operator::ADD:
					return lhs + rhs;
				case Monkey_job_operator::SUBTRACT:
					return lhs - rhs;
				case Monkey_job_operator::MULTIPLY:
					return lhs * rhs;
				case Monkey_job_operator::DIVIDE:
					return lhs / rhs;
				default:
					return -1;
			}
		}
		
		return std::get<int64_t>(value);
	}

	static bool branch_has_human(const Monkey_map& monkeys, const std::string& key)
	{
		if (key == "humn")
			return true;

		const auto& value = monkeys.at(key);
		if (value.index() == 0)
		{
			const Monkey_job& job = std::get<Monkey_job>(value);
			return branch_has_human(monkeys, job.lhs) || branch_has_human(monkeys, job.rhs);
		}

		return false;
	}

	static int64_t reverse_execute_job(const int64_t lhs, const int64_t rhs, const Monkey_job_operator op, const bool inverted = false)
	{
		switch (op)
		{
			case Monkey_job_operator::ADD:
				return !inverted ? lhs - rhs : rhs - lhs;
			case Monkey_job_operator::SUBTRACT:
				return !inverted ? lhs + rhs : lhs - rhs;
			case Monkey_job_operator::MULTIPLY:
				return !inverted ? lhs / rhs : rhs / lhs;
			case Monkey_job_operator::DIVIDE:
				return !inverted ? lhs * rhs : lhs / rhs;
			default:
				return -1;
		}
	}

	static int64_t reverse_solve_expression(const Monkey_map& monkeys, const std::string& key, int64_t human_number)
	{
		if (key == "humn")
			return human_number;

		const auto& value = monkeys.at(key);
		if (value.index() == 0)
		{
			const Monkey_job& job = std::get<Monkey_job>(value);
			if (branch_has_human(monkeys, job.lhs))
			{
				int64_t rhs = solve_expression(monkeys, job.rhs);
				human_number = reverse_execute_job(human_number, rhs, job.op);
				return reverse_solve_expression(monkeys, job.lhs, human_number);
			}
			else
			{
				int64_t lhs = solve_expression(monkeys, job.lhs);
				human_number = reverse_execute_job(lhs, human_number, job.op, true);
				return reverse_solve_expression(monkeys, job.rhs, human_number);
			}
		}

		return std::get<int64_t>(value);
	}

	std::string Day_21::part_1(const std::filesystem::path& input_path) const
	{
		Monkey_map monkeys = parse_input(input_path / "day_21.txt");

		int64_t result = solve_expression(monkeys, "root");

		return fmt::format("Day 21 Part 1 | 'root' yells: {}", result);
	}

	std::string Day_21::part_2(const std::filesystem::path& input_path) const
	{
		Monkey_map monkeys = parse_input(input_path / "day_21.txt");

		const Monkey_job& root = std::get<Monkey_job>(monkeys.at("root"));
		int64_t human_number = 0;
		if (branch_has_human(monkeys, root.lhs))
		{
			int64_t rhs = solve_expression(monkeys, root.rhs);
			human_number = reverse_solve_expression(monkeys, root.lhs, rhs);
		}
		else
		{
			int64_t lhs = solve_expression(monkeys, root.lhs);
			human_number = reverse_solve_expression(monkeys, root.rhs, lhs);
		}

		return fmt::format("Day 21 Part 2 | 'humn' should yell: {}", human_number);
	}

} // aoc
