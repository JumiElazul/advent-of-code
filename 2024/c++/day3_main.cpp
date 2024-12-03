#include "jumi_utils.h"
#include <iostream>
#include <fstream>
#include <regex>

constexpr const char* filepath = "input/day3_input.txt";

void part_one(const std::string& contents)
{
    std::regex pattern("mul\\(\\d+,\\d+\\)");
    auto begin = std::sregex_iterator(contents.begin(), contents.end(), pattern);
    auto end = std::sregex_iterator();

    std::vector<std::string> matches;
    for (std::sregex_iterator i = begin; i != end; ++i)
    {
        std::smatch match = *i;
        matches.push_back(match.str());
    }

    size_t result = 0;
    for (std::string& match : matches)
    {
        match = match.substr(4, match.size());
        match = match.substr(0, match.size() - 1);

        auto it = std::find(match.begin(), match.end(), ',');
        size_t pos = std::distance(match.begin(), it);

        size_t first = std::stoul(match.substr(0, pos));
        size_t second = std::stoul(match.substr(pos + 1));

        result += (first * second);

        // std::cout << "First: " << first << "   Second: " << second << '\n';
    }
    std::cout << "[Part One] Result is: " << result << '\n';
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::string contents = jumi::stringify_file(file);

    part_one(contents);
}
