#include "jumi_utils.h"
#include <string>
#include <vector>
#include <unordered_map>

#define START_NUM 235741
#define END_NUM 706948

bool str_has_pair(const std::string& str)
{
    for (int i = 0; i < str.size() - 1; ++i)
    {
        if (str[i] == str[i + 1])
            return true;
    }

    return false;
}

bool str_has_pair_p2(const std::string& str)
{
    bool found_exactly_two = false;

    int run_length = 1;  
    for (int i = 0; i < str.size() - 1; ++i)
    {
        if (str[i] == str[i + 1])
        {
            run_length++;
        }
        else
        {
            if (run_length == 2)
                found_exactly_two = true;

            run_length = 1;
        }
    }

    if (run_length == 2)
        found_exactly_two = true;

    return found_exactly_two;
}

bool str_is_ascending(const std::string& str)
{
    for (int i = 0; i < str.size() - 1; ++i)
    {
        if (str[i] - '0' > str[i + 1] - '0')
            return false;
    }

    return true;
}

void part_one()
{
    size_t count = 0;

    for (int i = START_NUM; i <= END_NUM; ++i)
    {
        std::string str = std::to_string(i);
        bool has_pair = str_has_pair(str);
        if (!has_pair) continue;

        bool is_ascending = str_is_ascending(str);
        if (!is_ascending) continue;

        ++count;
    }

    std::cout << "[Part One] The amount of passwords it could be is " << count << '\n';
}

void part_two()
{
    size_t count = 0;

    for (int i = START_NUM; i <= END_NUM; ++i)
    {
        std::string str = std::to_string(i);
        bool has_pair = str_has_pair_p2(str);
        if (!has_pair) continue;

        bool is_ascending = str_is_ascending(str);
        if (!is_ascending) continue;

        ++count;
    }

    std::cout << "[Part Two] The amount of passwords it could be is " << count << '\n';
}

int main()
{
    part_one();
    part_two();
}
