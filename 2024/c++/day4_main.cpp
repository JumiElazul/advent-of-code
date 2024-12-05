#include "jumi_utils.h"
#include <iostream>
#include <string>
#include <vector>

constexpr const char* filepath = "input/day4_input.txt";

void part_one(const std::vector<std::string>& contents)
{
    static const std::string target1 = "XMAS";
    static const std::string target2 = "SAMX";
    size_t count = 0;

    // Check all rows
    for (int row = 0; row < contents.size(); ++row)
    {
        for (int col = 0; col < contents[row].size() - 4; ++col)
        {
            if (contents[row].substr(col, 4) == target1 || contents[row].substr(col, 4) == target2)
            {
                ++count;
            }
        }
    }

    // Check all cols
    for (int col = 0; col < contents[0].size(); ++col)
    {
        for (int row = 0; row < contents[col].size() - 4; ++row)
        {
            std::string concat = std::string(1, contents[row][col]) + std::string(1, contents[row + 1][col]) + std::string(1, contents[row + 2][col]) + std::string(1, contents[row + 3][col]);
            if (concat == target1 || concat == target2)
            {
                ++count;
            }
        }
    }

    // Check diagonal down right
    for (int row = 0; row < contents[0].size() - 4; ++row)
    {
        for (int col = 0; col < contents[row].size() - 4; ++row)
        {
            char first = contents[row][col];
            char second = contents[row + 1][col + 1];
            char third = contents[row + 2][col + 2];
            char fourth = contents[row + 3][col + 3];
            std::string concat = std::string(1, first) + std::string(1, second) + std::string(1, third) + std::string(1, fourth);
            if (concat == target1 || concat == target2)
            {
                ++count;
            }
        }
    }

    // Check diagonal down left
    for (int row = 3; row < contents[0].size(); ++row)
    {
        for (int col = 3; col < contents[row].size(); ++row)
        {
            char first = contents[row][col];
            char second = contents[row - 1][col - 1];
            char third = contents[row - 2][col - 2];
            char fourth = contents[row - 3][col - 3];
            std::string concat = std::string(1, first) + std::string(1, second) + std::string(1, third) + std::string(1, fourth);
            if (concat == target1 || concat == target2)
            {
                ++count;
            }
        }
    }

    std::cout << "[Part One] The result is " << count << '\n';
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> contents = jumi::read_lines(file);
    part_one(contents);
}
