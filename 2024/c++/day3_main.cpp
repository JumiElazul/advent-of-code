#include "jumi_utils.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <regex>

constexpr const char* filepath = "input/day3_input.txt";

void part_one(const std::vector<std::string>& contents)
{
    std::regex pattern("mul\\(\\d+,\\d+\\)");
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> contents = jumi::read_lines(file);

    part_one(contents);
}
