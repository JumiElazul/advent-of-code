#include "jumi_utils.h"
#include <iostream>
#include <vector>
#include <deque>
#include <string>

std::deque<size_t> get_stones(const std::string& line)
{
    std::deque<size_t> stones;
    std::vector<std::string> split = jumi::split(line, ' ');
    for (const std::string& str : split)
    {
        stones.push_back(std::stoull(str));
    }
    return stones;
}

void part_one(std::deque<size_t>& stones)
{
    for (int i = 0; i < 25; ++i)
    {
        for (auto it = stones.begin(); it != stones.end(); ++it)
        {

        }
    }

    std::cout << "[Part One] The final number of stones is " << stones.size() << '\n';
}

int main()
{
    std::fstream file = jumi::open_file("input/day11_input.txt");
    std::string line = jumi::stringify_file(file);
    std::deque<size_t> stones = get_stones(line);
    part_one(stones);
}
