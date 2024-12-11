#include "jumi_utils.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

std::vector<size_t> get_stones(const std::string& line)
{
    std::vector<size_t> stones;
    std::vector<std::string> split = jumi::split(line, ' ');
    for (const std::string& str : split)
    {
        stones.push_back(std::stoull(str));
    }
    return stones;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& list)
{
    for (const auto& elem : list)
        os << elem << '\n';

    return os;
}

struct pair_hash
{
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

size_t recurse(size_t num, int blinks)
{
    static std::unordered_map<std::pair<size_t, int>, size_t, pair_hash> cache;

    if (blinks == 0)
        return 1;

    size_t& ref = cache[{ num, blinks }];
    if (ref != 0)
        return cache[{ num, blinks }];

    if (num == 0)
        return ref = recurse(1, blinks - 1);

    std::string str = std::to_string(num);
    if (str.size() % 2 == 0)
    {
        std::string first = str.substr(0, str.size() / 2);
        std::string second = str.substr(str.size() / 2, str.size() / 2);
        return ref = recurse(std::stoull(first), blinks - 1) + recurse(std::stoull(second), blinks - 1);
    }
    else
    {
        return ref = recurse(num * 2024, blinks - 1);
    }
}

void part_one(const std::vector<size_t>& stones)
{
    size_t result = 0;

    jumi::scoped_timer t("part_one");
    for (const size_t s : stones)
    {
        result += recurse(s, 25);
    }

    std::cout << "[Part One] The final number of stones is " << result << '\n';
}

void part_two(const std::vector<size_t>& stones)
{
    size_t result = 0;

    jumi::scoped_timer t("part_two");
    for (const size_t s : stones)
    {
        result += recurse(s, 75);
    }

    std::cout << "[Part Two] The final number of stones is " << result << '\n';
}

int main()
{
    std::fstream file = jumi::open_file("input/day11_input.txt");
    std::string line = jumi::stringify_file(file);
    std::vector<size_t> stones = get_stones(line);
    part_one(stones);
    part_two(stones);
}
