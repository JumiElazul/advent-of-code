#include "jumi_utils.h"

constexpr const char* filepath = "Day9_input.txt";

using jumi::operator<<;

std::vector<int32> get_difference_vec(const std::vector<int32>& vec)
{
    std::vector<int32> diff;

    for (size_t i = 1; i < vec.size(); ++i)
    {
        diff.emplace_back(vec[i] - vec[i - 1]);
    }

    return diff;
}

bool is_zero_vec(const std::vector<int32>& vec)
{
    for (const int32 num : vec)
    {
        if (num != 0)
        {
            return false;
        }
    }
    return vec.size() > 0 ? true : false;
}

int64 process_line(const std::vector<int32>& line)
{
    std::vector<std::vector<int32>> history;
    history.emplace_back(line);

    std::vector<int32> diff_vec = line;
    while (!is_zero_vec(diff_vec))
    {
        diff_vec = get_difference_vec(diff_vec);
        history.emplace_back(diff_vec);
    }

    int64 diff = 0;
    for (size_t i = history.size() - 1; i >= 0; --i)
    {
        history[i].push_back(diff);
    }
}

int64 process_input(const std::vector<std::vector<int32>>& input)
{
    int64 sum = 0;

    for (const std::vector<int32>& vec : input)
    {
        sum += process_line(vec);
    }

    return sum;
}

std::vector<std::vector<int32>> convert_to_int_vec(const std::vector<std::string>& vec)
{
    std::vector<std::vector<int32>> numbers;

    for (const std::string& line : vec)
    {
        std::vector<std::string> split = jumi::split(line, ' ');
        std::vector<int32> new_vec;

        for (const std::string& c : split)
        {
            new_vec.emplace_back(std::stoi(c));
        }

        numbers.emplace_back(new_vec);
    }

    return numbers;
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> contents = jumi::vectorize_file(file);
    std::vector<std::vector<int32>> numbers = convert_to_int_vec(contents);
    process_line(numbers[0]);

    return 0;
}
