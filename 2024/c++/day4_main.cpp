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
    for (const std::string& line : contents)
    {
        for (int i = 0; i < line.size() - 3; ++i)
        {
            std::string slice = line.substr(i, 4);
            if (slice == target1 || slice == target2)
            {
                ++count;
            }
        }
    }

    // Check all cols
    for (int i = 0; i < contents[0].size(); ++i)
    {
        for (int j = 0; j < contents.size() - 3; ++j)
        {
            std::stringstream ss;
            ss << contents[j][i] << contents[j + 1][i] << contents[j + 2][i] << contents[j + 3][i];
            std::string slice = ss.str();
            if (slice == target1 || slice == target2)
            {
                ++count;
            }
        }
    }

    // Check diag down right
    for (int i = 0; i < contents.size() - 3; ++i)
    {
        for (int j = 0; j < contents[i].size() - 3; ++j)
        {
            std::stringstream ss;
            ss << contents[i][j] << contents[i + 1][j + 1] << contents[i + 2][j + 2] << contents[i + 3][j + 3];
            std::string slice = ss.str();
            if (slice == target1 || slice == target2)
            {
                ++count;
            }
        }
    }

    // Check diag down left
    for (int i = 0; i < contents.size() - 3; ++i)
    {
        for (int j = 3; j < contents.size(); ++j)
        {
            std::stringstream ss;
            ss << contents[i][j] << contents[i + 1][j - 1] << contents[i + 2][j - 2] << contents[i + 3][j - 3];
            std::string slice = ss.str();
            if (slice == target1 || slice == target2)
            {
                ++count;
            }
        }
    }

    std::cout << "[Part One] The result is " << count << '\n';
}

void part_two(const std::vector<std::string>& contents)
{
    static const std::string target1 = "MAS";
    static const std::string target2 = "SAM";
    size_t count = 0;

    for (int row = 1; row < contents.size() - 1; ++row)
    {
        for (int col = 1; col < contents[row].size() - 1; ++col)
        {
            if (contents[row][col] == 'A')
            {
                std::stringstream dr;
                dr << contents[row - 1][col - 1] << contents[row][col] << contents[row + 1][col + 1];

                std::stringstream dl;
                dl << contents[row - 1][col + 1] << contents[row][col] << contents[row + 1][col - 1];

                std::string dr_str = dr.str();
                std::string dl_str = dl.str();

                if ((dr_str == target1 || dr_str == target2) && (dl_str == target1 || dl_str == target2))
                {
                    ++count;
                }
            }
        }
    }

    std::cout << "[Part Two] The result is " << count << '\n';
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> contents = jumi::read_lines(file);
    part_one(contents);
    part_two(contents);
}
