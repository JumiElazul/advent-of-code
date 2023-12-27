#include "jumi_utils.h"
#include <queue>
#include <set>
#include <unordered_map>

constexpr const char* filepath { "Day10_input.txt" };
constexpr const char* test_filepath { "Day10_input2.txt" };

std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& pair)
{
    os << pair.first << ',' << pair.second;
    return os;
}

std::pair<int, int> get_start_pos(const std::vector<std::string>& lines)
{
    std::pair<int, int> result;
    bool found = false;

    for (size_t i = 0; i < lines.size(); ++i)
    {
        if (found) break;
        for (size_t j = 0; j < lines[i].size(); ++j)
        {
            if (found) break;
            if (lines[i][j] == 'S')
            {
                result.first = static_cast<int>(i);
                result.second = static_cast<int>(j);
                found = true;
            }
        }
    }
    return result;
}

enum Direction
{
    North,
    South,
    East,
    West
};

std::unordered_map<char, std::vector<Direction>> valid_connections
{
    {'S', { North, South, East, West } },
    {'|', { North, South             } },
    {'-', { East,  West              } },
    {'L', { North, East              } },
    {'J', { North, West              } },
    {'7', { South, West              } },
    {'F', { South, East              } },
    {'.', {                          } }
};

bool is_valid_location(const std::vector<std::string>& input, const std::pair<int, int>& from, const std::pair<int, int>& to)
{
    if (to.first >= input.size() || to.first < 0 || to.second >= input[0].size() || to.second < 0)
    {
        return false;
    }

    char current_tile = input[from.first][from.second];
    char next_tile = input[to.first][to.second];

    Direction direction;
    if (from.first == to.first)
    {
        direction = (from.second < to.second) ? East : West;
    }
    else
    {
        direction = (from.first < to.first) ? South : North;
    }

    if (std::find(valid_connections[current_tile].begin(), valid_connections[current_tile].end(), direction) == valid_connections[current_tile].end())
    {
        return false;
    }

    Direction opposite;
    switch (direction) 
    {
        case North:
        {
            opposite = South;
            break;
        }
        case South:
        {
            opposite = North;
            break;
        }
        case East:
        {
            opposite = West;
            break;
        }
        case West:
        {
            opposite = East;
            break;
        }
    }

    return find(valid_connections[next_tile].begin(), valid_connections[next_tile].end(), opposite) != valid_connections[next_tile].end();
}

int process_input(const std::vector<std::string>& input, const std::pair<int, int>& start_pos)
{
    std::queue<std::pair<int, int>> queue;
    std::set<std::pair<int, int>> set;
    queue.push(start_pos);
    set.insert(start_pos);

    while (!queue.empty())
    {
        std::pair<int, int> location = queue.front();
        queue.pop();

        std::pair<int, int> west  { location.first, location.second - 1 };
        std::pair<int, int> east  { location.first, location.second + 1 };
        std::pair<int, int> north { location.first - 1, location.second };
        std::pair<int, int> south { location.first + 1, location.second };

        if (is_valid_location(input, location, west))
        {
            if (auto find = set.find(west) == set.end())
            {
                set.insert(west);
                queue.push(west);
            }
        }
        if (is_valid_location(input, location, east))
        {
            if (auto find = set.find(east) == set.end())
            {
                set.insert(east);
                queue.push(east);
            }
        }
        if (is_valid_location(input, location, north))
        {
            if (auto find = set.find(north) == set.end())
            {
                set.insert(north);
                queue.push(north);
            }
        }
        if (is_valid_location(input, location, south))
        {
            if (auto find = set.find(south) == set.end())
            {
                set.insert(south);
                queue.push(south);
            }
        }
    }

    return set.size() / 2;
}

int main()
{
    // Part 1
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> lines = jumi::read_lines(file);
    std::pair<int, int> start_pos = get_start_pos(lines);
    int result = process_input(lines, start_pos);

    for (const auto& line : lines)
    {
        std::cout << line << '\n';
    }

    std::cout << "Part1 result: " << result << '\n';

    // Part 2

    return 0;
}
