#include "jumi_utils.h"
#include <iostream>
#include <numeric>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

constexpr const char* filepath = "input/day10_input.txt";

struct coord
{
    int x;
    int y;

    bool operator==(const coord &other) const 
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const coord &other) const 
    {
        return x != other.x || y != other.y;
    }
};

struct pair_hash 
{
    std::size_t operator()(const coord& c) const 
    {
        return std::hash<int>()(c.x) ^ (std::hash<int>()(c.y) << 1);
    }
};

bool is_in_bounds(const coord& p, const std::vector<std::string>& grid)
{
    return p.x >= 0
        && p.y >= 0
        && p.x < grid[0].size()
        && p.y < grid.size();
}

void part_one(const std::vector<std::string>& grid)
{
    static coord directions[4] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
    size_t result = 0;

    for (int row = 0; row < grid.size(); ++row)
    {
        for (int col = 0; col < grid[row].size(); ++col)
        {
            if (grid[row][col] == '0')
            {
                coord start_loc = { col, row };

                std::unordered_map<coord, bool, pair_hash> visited;
                std::queue<coord> queue;

                visited[start_loc] = true;
                queue.push(start_loc);

                while (!queue.empty())
                {
                    coord curr = queue.front();
                    queue.pop();

                    char current_char = grid[curr.y][curr.x];

                    if (current_char == '9')
                    {
                        ++result;
                        continue;
                    }

                    char next_char = current_char + 1;

                    for (const auto& d : directions)
                    {
                        coord new_coord = { curr.x + d.x, curr.y + d.y };
                        if (is_in_bounds(new_coord, grid) && !visited[new_coord] && grid[new_coord.y][new_coord.x] == next_char)
                        {
                            visited[new_coord] = true;
                            queue.push(new_coord);
                        }
                    }
                }
            }
        }
    }

    std::cout << "[Part One] The final 9's that are able to be reached is " << result << '\n';
}

void part_two(const std::vector<std::string>& grid)
{
    static coord directions[4] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
    size_t result = 0;

    for (int row = 0; row < grid.size(); ++row)
    {
        for (int col = 0; col < grid[row].size(); ++col)
        {
            if (grid[row][col] == '0')
            {
                coord start_loc = { col, row };

                std::queue<coord> queue;

                queue.push(start_loc);

                while (!queue.empty())
                {
                    coord curr = queue.front();
                    queue.pop();

                    char current_char = grid[curr.y][curr.x];

                    if (current_char == '9')
                    {
                        ++result;
                        continue;
                    }

                    char next_char = current_char + 1;

                    for (const auto& d : directions)
                    {
                        coord new_coord = { curr.x + d.x, curr.y + d.y };
                        if (is_in_bounds(new_coord, grid) && grid[new_coord.y][new_coord.x] == next_char)
                        {
                            queue.push(new_coord);
                        }
                    }
                }
            }
        }
    }


    std::cout << "[Part Two] The final total trailhead scores is " << result << '\n';
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> contents = jumi::read_lines(file);
    part_one(contents);
    part_two(contents);
}
