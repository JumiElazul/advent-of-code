#include "jumi_utils.h"
#include <fstream>
#include <vector>
#include <string>

void part_one(const std::vector<std::string>& lines)
{
    int fuel_required = 0;
    for (const std::string& str : lines)
    {
        fuel_required += std::stoi(str) / 3 - 2;
    }

    std::cout << "[Part One] Final fuel needed: " << fuel_required << '\n';
}

void part_two(const std::vector<std::string>& lines)
{
    int fuel_required = 0;

    auto rec = [&](auto& self, int fuel) -> int {
        if (fuel <= 0)
            return 0;

        fuel_required += fuel;
        return self(self, fuel / 3 - 2);
    };

    for (const std::string& str : lines)
    {
        fuel_required += rec(rec, std::stoi(str) / 3 - 2);
    }

    std::cout << "[Part Two] Final fuel needed: " << fuel_required << '\n';
}

int main()
{
    std::fstream file = jumi::open_file("input/day1_input.txt");
    std::vector<std::string> lines = jumi::read_lines(file);
    part_one(lines);
    part_two(lines);
}

