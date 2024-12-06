#include "jumi_utils.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <vector>

constexpr const char* filepath = "input/day5_input.txt";

std::unordered_map<int, std::vector<int>> build_print_map(const std::vector<std::string>& contents)
{
    std::unordered_map<int, std::vector<int>> print_map;
    for (const std::string& line : contents)
    {
        auto it = line.find('|');
        if (it != std::string::npos)
        {
            std::string first = line.substr(0, it);
            std::string second = line.substr(it + 1);
            int f = std::stoi(first);
            int s = std::stoi(second);
            print_map[f].push_back(s);
        }
    }

    return print_map;
}

std::vector<std::vector<int>> build_instruction_map(const std::vector<std::string>& contents)
{
    std::vector<std::vector<int>> instruction_map;

    for (const std::string& line : contents)
    {
        std::vector<int> v;
        size_t curr = 0;
        size_t find = 0;

        if (line.find(',') == std::string::npos)
            continue;

        while ((find = line.find(',', curr)) != std::string::npos)
        {
            int val = std::stoi(line.substr(curr, find));
            v.emplace_back(val);
            curr = find + 1;
        }
        int val = std::stoi(line.substr(curr));
        v.emplace_back(val);

        instruction_map.push_back(v);
    }

    return instruction_map;
}

void part_one(const std::unordered_map<int, std::vector<int>>& print_map, const std::vector<std::vector<int>>& instruction_map)
{
    size_t sum = 0;
    for (const std::vector<int>& instruction_list : instruction_map)
    {
        bool valid = true;
        std::vector<int> printed;

        for (const int i : instruction_list)
        {
            const std::vector<int>& rules = print_map.at(i);
            for (const int rule : rules)
            {
                if (std::find(printed.begin(), printed.end(), rule) != printed.end())
                {
                    valid = false;
                    break;
                }
            }

            if (!valid)
                break;

            printed.push_back(i);
        }
        if (valid)
        {
            size_t index = printed.size() / 2;
            sum += printed.at(index);
        }
    }

    std::cout << "[Part One] The final result is " << sum << '\n';
}

void part_two(const std::unordered_map<int, std::vector<int>>& print_map, const std::vector<std::vector<int>>& instruction_map)
{

}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> contents = jumi::read_lines(file);
    std::unordered_map<int, std::vector<int>> print_map = build_print_map(contents);
    std::vector<std::vector<int>> instruction_map = build_instruction_map(contents);

    part_one(print_map, instruction_map);
    part_two(print_map, instruction_map);
}
