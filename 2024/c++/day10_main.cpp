#include "jumi_utils.h"
#include <iostream>
#include <string>
#include <vector>

constexpr const char* filepath = "input/day10_input.txt";

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> contents = jumi::read_lines(file);
}
