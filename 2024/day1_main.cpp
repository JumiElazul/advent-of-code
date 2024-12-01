#include "jumi_utils.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <map>

constexpr const char* input = { "input/day1_input.txt" };

struct input_t
{
    std::vector<int> left;
    std::vector<int> right;
};

input_t read_input()
{
    std::fstream file(input);
    input_t lists;

    std::string line;
    std::string delim = "   ";
    while (std::getline(file, line))
    {
        std::vector<std::string> split = jumi::split(line, delim);
        lists.left.push_back(std::stoi(split.at(0)));
        lists.right.push_back(std::stoi(split.at(1)));
    }

    return lists;
}

void part_one()
{
    input_t lists = read_input();

    std::sort(lists.left.begin(), lists.left.end());
    std::sort(lists.right.begin(), lists.right.end());

    int list_difference = 0;

    for (size_t i = 0; i < lists.left.size(); ++i)
    {
        list_difference += std::abs(lists.left.at(i) - lists.right.at(i));
    }

    std::cout << "[Part One] The final difference between the two lists is: " << list_difference << '\n';
}

void part_two()
{
    input_t lists = read_input();

    std::map<int, int> histogram;

    for (size_t i = 0; i < lists.left.size(); ++i)
        histogram[lists.right[i]]++;

    size_t list_diff = 0;
    for (size_t i = 0; i < lists.left.size(); ++i)
    {
        list_diff += static_cast<size_t>(std::abs(lists.left[i] * histogram[lists.left[i]]));
    }

    std::cout << "[Part Two] The final difference between the two lists is: " << list_diff << '\n';
}

int main()
{
    part_two();
}
