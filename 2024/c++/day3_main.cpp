#include "jumi_utils.h"
#include <iostream>
#include <fstream>
#include <regex>

constexpr const char* filepath = "input/day3_input.txt";

struct match_pair
{
    size_t first_operand;
    size_t second_operand;
};

match_pair format_match_str(std::string& match)
{
    match = match.substr(4, match.size());
    match = match.substr(0, match.size() - 1);

    auto it = std::find(match.begin(), match.end(), ',');
    size_t pos = std::distance(match.begin(), it);

    size_t first = std::stoul(match.substr(0, pos));
    size_t second = std::stoul(match.substr(pos + 1));
    return match_pair{ first, second };
}

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
       match_pair pair = format_match_str(match);
       result += pair.first_operand * pair.second_operand;

        // std::cout << "First: " << first << "   Second: " << second << '\n';
    }
    std::cout << "[Part One] Result is: " << result << '\n';
}

void part_two(std::string contents)
{
    std::regex pattern("(do\\(\\)|don't\\(\\)|mul\\(\\d+,\\d+\\))");
    bool do_mul = true;

    auto begin = std::sregex_iterator(contents.begin(), contents.end(), pattern);
    auto end = std::sregex_iterator();

    size_t result = 0;
    for (std::sregex_iterator it = begin; it != end; ++it)
    {
        std::smatch match = *it;
        std::string match_str = match.str();

        if (match_str == "do()")
        {
            do_mul = true;
        }
        else if (match_str == "don't()")
        {
            do_mul = false;
        }
        else if (match_str.substr(0, 4) == "mul(")
        {
            if (do_mul)
            {
                match_pair pair = format_match_str(match_str);
                result += pair.first_operand * pair.second_operand;
            }
        }
    }

    std::cout << "[Part Two] Result is: " << result << '\n';
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::string contents = jumi::stringify_file(file);

    // part_one(contents);
    part_two(contents);
}
