#include "jumi_utils.h"
#include <iostream>
#include <string>
#include <vector>

constexpr const char* filepath = "input/day7_input.txt";

int main()
{
    std::fstream file(filepath);
    std::vector<std::string> contents = jumi::read_lines(file);

    for (const auto& l : contents)
    {
        std::cout << l << '\n';
    }
    std::cout << '\n';
}
