#include "jumi_utils.h"
#include <cassert>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <sstream>

constexpr const char* filepath = "Day6_input.txt";

struct race
{
    int32 time = 0;
    int32 dist = 0;
};

std::ostream& operator<<(std::ostream& os, const race& race)
{
    os << race.time << ',' << race.dist;
    return os;
}

std::vector<race> parse_input(std::fstream& fstream)
{
    std::vector<race> races;
    std::string line;
    std::vector<int> times;
    std::vector<int> distances;

    while (std::getline(fstream, line))
    {
        std::string category = line.substr(0, line.find_first_of(':'));
        size_t first_index = line.find_first_of("0123456789");
        size_t last_index = line.find_last_of("0123456789");
        line = line.substr(first_index, last_index + 1);

        std::istringstream iss(line);
        int32 num;

        while (iss >> num)
        {
            if (category == "Time")
                times.push_back(num);
            else if (category == "Distance")
                distances.push_back(num);
        }
    }

    assert(times.size() == distances.size());

    for (size_t i = 0; i < times.size(); ++i)
    {
        races.emplace_back(times[i], distances[i]);
    }

    return races;
}

bool check_win(const race& race, int32 hold_time)
{
    int32 remaining_time = race.time - hold_time;
    int32 speed = hold_time;
    return speed * remaining_time > race.dist;
}

int32 calculate_wins(const race& race)
{
    int32 ways_to_win = 0;
    int32 iterations = race.time;

    for (size_t i = 1; i < static_cast<size_t>(iterations); ++i)
    {
        if (check_win(race, static_cast<int32>(i)))
        {
            ++ways_to_win;
        }
    }

    return ways_to_win;
}

int32 calculate_multiply_wins(const std::vector<race>& races)
{
    int32 sum = 1;

    for (const race& race : races)
    {
        int32 wins = calculate_wins(race);
        sum *= wins;
    }

    return sum;
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<race> races { parse_input(file) };
    int32 multiply_win_result { calculate_multiply_wins(races) };
    std::cout << "Part1 result: " << multiply_win_result << '\n';

    return 0;
}
