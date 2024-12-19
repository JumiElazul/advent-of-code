#include "jumi_utils.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <variant>
#include <vector>
#include <unordered_set>

std::unordered_map<int, std::vector<int>> build_adj_list(const std::vector<std::string>& contents)
{
    std::unordered_map<int, std::vector<int>> adj_list;
    for (const std::string& line : contents)
    {
        auto it = line.find('|');
        if (it != std::string::npos)
        {
            std::string first = line.substr(0, it);
            std::string second = line.substr(it + 1);
            int f = std::stoi(first);
            int s = std::stoi(second);
            adj_list[f].push_back(s);
        }
    }

    return adj_list;
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

bool is_valid_instruction(const std::unordered_map<int, std::vector<int>>& adj_list, const std::vector<int>& instruction_list)
{
    std::vector<int> printed;
    bool valid = true;

    for (const int i : instruction_list)
    {
        const std::vector<int>& rules = adj_list.at(i);
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
    return valid;
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& list)
{
    for (const int i : list)
        os << i << ' ';

    return os;
}

void part_one(const std::unordered_map<int, std::vector<int>>& adj_list, const std::vector<std::vector<int>>& instruction_map)
{
    size_t sum = 0;
    for (const std::vector<int>& instruction_list : instruction_map)
    {
        bool valid = is_valid_instruction(adj_list, instruction_list);
        if (valid)
        {
            sum += instruction_list.at(instruction_list.size() / 2);
        }
    }

    std::cout << "[Part One] The final result is " << sum << '\n';
}

void part_two(const std::unordered_map<int, std::vector<int>>& adj_list, const std::vector<std::vector<int>>& instruction_map)
{
    std::vector<std::vector<int>> fixed_instructions;
    fixed_instructions.reserve(128);

    for (const std::vector<int>& instruction_list : instruction_map)
    {
        bool valid = is_valid_instruction(adj_list, instruction_list);
        if (!valid)
        {
            std::vector<int> top_sort;
            std::unordered_set<int> visited;
            std::unordered_set<int> recursion_stack;
            top_sort.reserve(32);

            auto dfs = [&](auto& self, int node) -> void {
                if (recursion_stack.find(node) != visited.end())
                    throw std::runtime_error("Graph contains a cycle");

                if (visited.find(node) != visited.end())
                    return;

                recursion_stack.emplace(node);
                visited.emplace(node);

                auto it = adj_list.find(node);
                if (it != adj_list.end())
                {
                    for (const int neighbour : it->second)
                    {
                        if (std::find(instruction_list.begin(), instruction_list.end(), neighbour) != instruction_list.end())
                        {
                            self(self, neighbour);
                        }
                    }
                }

                recursion_stack.erase(node);
                top_sort.push_back(node);
            };

            for (const int i : instruction_list)
            {
                dfs(dfs, i);
            }

            std::reverse(top_sort.begin(), top_sort.end());
            fixed_instructions.push_back(top_sort);
        }

    }

    size_t sum = 0;
    for (const std::vector<int>& v : fixed_instructions)
    {
        const size_t& s = v.size();
        if (!v.empty())
            sum += v[s / 2];
    }

    std::cout << "[Part Two] The final result is " << sum << '\n';
}

int main()
{
    std::fstream file = jumi::open_file("input/day5_input.txt");
    std::vector<std::string> contents = jumi::read_lines(file);
    std::unordered_map<int, std::vector<int>> adj_list = build_adj_list(contents);
    std::vector<std::vector<int>> instruction_map = build_instruction_map(contents);

    part_one(adj_list, instruction_map);
    part_two(adj_list, instruction_map);
}
