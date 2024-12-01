#include "jumi_utils.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

constexpr const char* input = { "input/day1_input.txt" };

int main()
{
    std::fstream file(input);

    std::vector<int> first_list;
    std::vector<int> second_list;

    std::string line;
    std::string delim = "   ";
    while (std::getline(file, line))
    {
        std::vector<std::string> split = jumi::split(line, delim);
        first_list.push_back(std::stoi(split.at(0)));
        second_list.push_back(std::stoi(split.at(1)));
    }

    std::sort(first_list.begin(), first_list.end());
    std::sort(second_list.begin(), second_list.end());

    size_t size = first_list.size();
    int list_difference = 0;

    for (size_t i = 0; i < size; ++i)
    {
        int diff = std::abs(first_list.at(i) - second_list.at(i));
        list_difference += diff;
    }

    std::cout << "The final difference between the two lists is: " << list_difference << '\n';
}
