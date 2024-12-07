#include "jumi_utils.h"
#include <iostream>
#include <string>
#include <vector>

constexpr const char* filepath = "input/day7_input.txt";

struct equation
{
    size_t target;
    std::vector<size_t> operands;
};

std::ostream& operator<<(std::ostream& os, const equation& eq)
{
    os << eq.target << ": ";
    for (const size_t i : eq.operands)
    {
        os << i << ' ';
    }
    return os;
}

std::vector<equation> get_operations_from_input(const std::vector<std::string>& contents)
{
    std::vector<equation> eqs;
    for (const std::string& line : contents)
    {
        equation new_eq;
        size_t index = line.find(':');
        std::string slice = line.substr(0, index);
        new_eq.target = std::stoul(slice);

        index = line.find_first_of(' ');
        std::string chopped_str = line.substr(index + 1);
        std::vector<std::string> split = jumi::split(chopped_str, ' ');
        for (const std::string& num : split)
        {
            new_eq.operands.push_back(std::stoul(num));
        }
        eqs.push_back(new_eq);
    }
    return eqs;
}

void part_one(const std::vector<std::string>& contents)
{
    std::vector<equation> equations = get_operations_from_input(contents);

    for (const auto& eq : equations)
    {

    }
}

int main()
{
    std::fstream file(filepath);
    std::vector<std::string> contents = jumi::read_lines(file);
    part_one(contents);
}
