#include "jumi_utils.h"
#include <cassert>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <sstream>

constexpr const char* filepath = "Day6_input.txt";

struct race
{
    int64 time = 0;
    int64 dist = 0;
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
    std::vector<int64> times;
    std::vector<int64> distances;

    while (std::getline(fstream, line))
    {
        std::string category = line.substr(0, line.find_first_of(':'));
        size_t first_index = line.find_first_of("0123456789");
        size_t last_index = line.find_last_of("0123456789");
        line = line.substr(first_index, last_index + 1);

        std::istringstream iss(line);
        int64 num;

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

bool check_win(const race& race, int64 hold_time)
{
    int64 remaining_time = race.time - hold_time;
    int64 speed = hold_time;
    return speed * remaining_time > race.dist;
}

int64 calculate_wins(const race& race)
{
    int64 first_win_hold_time = 0;
    int64 last_win_hold_time = 0;

    // Find the first way to win
    for (size_t i = 1; i < static_cast<size_t>(race.time); ++i)
    {
        if (check_win(race, static_cast<int64>(i)))
        {
            first_win_hold_time = static_cast<int64>(i);
            break;
        }
    }

    for (size_t i = static_cast<size_t>(race.time); i >= 1; --i)
    {
        if (check_win(race, static_cast<int64>(i)))
        {
            last_win_hold_time = static_cast<int64>(i);
            break;
        }
    }

    int64 ways_to_win = last_win_hold_time - first_win_hold_time + 1;
    return ways_to_win;
}

int64 calculate_multiply_wins(const std::vector<race>& races)
{
    int64 sum = 1;

    for (const race& race : races)
    {
        int64 wins = calculate_wins(race);
        sum *= wins;
    }

    return sum;
}

race parse_input_pt2(std::fstream& file)
{
    std::string time;
    std::string distance;
    std::string line;

    while (std::getline(file, line))
    {
        std::string category = line.substr(0, line.find_first_of(':'));
        size_t first_index = line.find_first_of("0123456789");
        size_t last_index = line.find_last_of("0123456789");
        line = line.substr(first_index, last_index + 1);

        for (size_t i = 0; i < line.length(); ++i)
        {
            if (std::isdigit(line[i]))
            {
                if (category == "Time")
                {
                    time += line[i];
                }
                else if (category == "Distance")
                {
                    distance += line[i];
                }
            }
        }
    }

    return { std::stoll(time), std::stoll(distance) };
}

int main()
{
    // ----------------------------------------
    // Part 1
    // ----------------------------------------
    std::fstream file = jumi::open_file(filepath);
    std::vector<race> races { parse_input(file) };
    int64 multiply_win_result { calculate_multiply_wins(races) };
    std::cout << "Part1 result: " << multiply_win_result << '\n';

    // ----------------------------------------
    // Part 2
    // ----------------------------------------
    file = jumi::open_file(filepath);
    race race = parse_input_pt2(file);
    int64 wins = calculate_wins(race);
    std::cout << "Part2 result: " << wins << '\n';

    return 0;
}
