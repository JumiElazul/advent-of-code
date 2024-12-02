#include "jumi_utils.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>
#include <vector>

constexpr const char* input = { "input/day2_input.txt" };

bool is_safe(std::vector<int>& numbers)
{
    if (numbers.size() < 2)
        return false;

    int initial_diff = numbers[1] - numbers[0];

    if (initial_diff == 0)
        return false;

    bool ascending = initial_diff > 0;
    bool is_valid = true;

    for (size_t i = 0; i < numbers.size() - 1; ++i)
    {
        int diff = numbers[i + 1] - numbers[i];

        if (diff == 0)
        {
            is_valid = false;
            break;
        }

        if ((ascending && diff <= 0) || (!ascending && diff >= 0))
        {
            is_valid = false;
            break;
        }

        int abs_diff = std::abs(diff);
        if (abs_diff < 1 || abs_diff > 3)
        {
            is_valid = false;
            break;
        }
    }

    if (is_valid)
        return true;
    return false;
}

void part_one(const std::vector<std::string>& input)
{
    int safety_count = 0;

    for (const std::string& line : input)
    {
        std::vector<int> numbers;
        int start = 0;
        int end = 0;

        while ((end = line.find(' ', start)) != std::string::npos)
        {
            numbers.emplace_back(std::stoi(line.substr(start, end - start)));
            start = end + 1;
        }
        numbers.emplace_back(std::stoi(line.substr(start, end - start)));

        if (is_safe(numbers))
            ++safety_count;
    }

    std::cout << "[Part One] Final safety count is: " << safety_count << '\n';
}

void part_two(const std::vector<std::string>& input)
{
    int safety_count = 0;

    for (const std::string& line : input)
    {
        std::vector<int> numbers;
        int start = 0;
        int end = 0;

        while ((end = line.find(' ', start)) != std::string::npos)
        {
            numbers.emplace_back(std::stoi(line.substr(start, end - start)));
            start = end + 1;
        }
        numbers.emplace_back(std::stoi(line.substr(start, end - start)));

        if (is_safe(numbers))
        {
            ++safety_count;
        }
        else
        {
            for (size_t i = 0; i < numbers.size(); ++i)
            {
                std::vector<int> modified_numbers = numbers;
                modified_numbers.erase(modified_numbers.begin() + i);

                if (modified_numbers.size() >= 2 && is_safe(modified_numbers))
                {
                    ++safety_count;
                    break;
                }
            }
        }
    }

    std::cout << "[Part Two] Final safety count is: " << safety_count << '\n';
}

int main()
{
    std::fstream file(input);
    std::vector<std::string> lines = jumi::read_lines(file);
    part_one(lines);
    part_two(lines);

}
