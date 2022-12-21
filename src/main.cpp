#include "utility/day.h"
#include "day_1/day_1.h"
#include "day_2/day_2.h"
#include "day_3/day_3.h"
#include "day_4/day_4.h"
#include "day_5/day_5.h"
#include "day_6/day_6.h"
#include "day_7/day_7.h"
#include "day_8/day_8.h"
#include "day_9/day_9.h"
#include "day_10/day_10.h"
#include "day_11/day_11.h"
#include "day_12/day_12.h"
#include "day_13/day_13.h"
#include "day_14/day_14.h"
#include "day_15/day_15.h"
#include "day_16/day_16.h"
#include "day_17/day_17.h"
#include "day_18/day_18.h"
#include "day_19/day_19.h"
#include "day_20/day_20.h"
#include "day_21/day_21.h"
#include "day_22/day_22.h"

#include "fmt/format.h"

#include <iostream>
#include <filesystem>
#include <vector>
#include <memory>
#include <chrono>
#include <cstdint>
#include <stdexcept>

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            throw std::invalid_argument("A path to a folder with inputs is required");
        }

        std::filesystem::path input_root = argv[1];

        // Create a list of solutions
        std::vector<std::unique_ptr<aoc::Day>> solutions;
        solutions.reserve(25);

        solutions.push_back(std::make_unique<aoc::Day_1>());
        solutions.push_back(std::make_unique<aoc::Day_2>());
        solutions.push_back(std::make_unique<aoc::Day_3>());
        solutions.push_back(std::make_unique<aoc::Day_4>());
        solutions.push_back(std::make_unique<aoc::Day_5>());
        solutions.push_back(std::make_unique<aoc::Day_6>());
        solutions.push_back(std::make_unique<aoc::Day_7>());
        solutions.push_back(std::make_unique<aoc::Day_8>());
        solutions.push_back(std::make_unique<aoc::Day_9>());
        solutions.push_back(std::make_unique<aoc::Day_10>());
        solutions.push_back(std::make_unique<aoc::Day_11>());
        // solutions.push_back(std::make_unique<aoc::Day_12>());
        solutions.push_back(std::make_unique<aoc::Day_13>());
        // solutions.push_back(std::make_unique<aoc::Day_14>());
        // solutions.push_back(std::make_unique<aoc::Day_15>());
        // solutions.push_back(std::make_unique<aoc::Day_16>());
        solutions.push_back(std::make_unique<aoc::Day_17>());
        solutions.push_back(std::make_unique<aoc::Day_18>());
        // solutions.push_back(std::make_unique<aoc::Day_19>());
        // solutions.push_back(std::make_unique<aoc::Day_20>());
        solutions.push_back(std::make_unique<aoc::Day_21>());
        solutions.push_back(std::make_unique<aoc::Day_22>());

        uint64_t total_runtime = 0;
        for (const auto& day : solutions)
        {
            // Part 1
            auto part_1_begin = std::chrono::high_resolution_clock::now();
            std::string part_1_output = day->part_1(input_root);
            auto part_1_end = std::chrono::high_resolution_clock::now();
            uint64_t part_1_runtime = std::chrono::duration_cast<std::chrono::milliseconds>(part_1_end - part_1_begin).count();

            // Part 2
            auto part_2_begin = std::chrono::high_resolution_clock::now();
            std::string part_2_output = day->part_2(input_root);
            auto part_2_end = std::chrono::high_resolution_clock::now();
            uint64_t part_2_runtime = std::chrono::duration_cast<std::chrono::milliseconds>(part_2_end - part_2_begin).count();

            // Print results
            std::cout << fmt::format("{} | Runtime: {}ms\n{} | Runtime: {}ms\n",
                part_1_output, part_1_runtime, part_2_output, part_2_runtime);

            // Add times to total time
            total_runtime += (part_1_runtime + part_2_runtime);
        }

        std::cout << fmt::format("Total runtime: {}ms\n", total_runtime);
    }
    catch (const std::exception& e)
    {
        std::cerr << fmt::format("Error: {}\n", e.what());
        return 1;
    }

    return 0;
}
