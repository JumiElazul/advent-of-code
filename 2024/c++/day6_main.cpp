#include "jumi_utils.h"
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <set>
#include <unordered_set>

constexpr const char* filepath = "input/day6_input.txt";

using grid_pos = std::pair<int, int>;

// struct pair_hash 
// {
//     size_t operator()(const std::pair<int, int>& p) const 
//     {
//         return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
//     }
// };

enum class direction
{
    north,
    south,
    east,
    west
};

grid_pos get_starting_pos(const std::vector<std::string>& grid)
{
    grid_pos start_pos;
    for (int row = 0; row < grid.size(); ++row)
    {
        auto col = grid[row].find('^');
        if (col != std::string::npos)
        {
            start_pos.first = row;
            start_pos.second = col;
        }
    }
    return start_pos;
}

direction switch_direction(direction dir)
{
    switch (dir)
    {
        case direction::north: return direction::east;
        case direction::south: return direction::west;
        case direction::east: return direction::south;
        case direction::west: return direction::north;
    }
    throw std::runtime_error("something is wrong");
}

bool is_in_bounds(const grid_pos& pos, const std::vector<std::string>& grid)
{
    return pos.first < grid.size() && pos.first >= 0 && pos.second < grid[0].size() && pos.second >= 0;
}

bool check_collision(direction dir, const grid_pos& pos, const std::vector<std::string>& grid)
{
    switch (dir)
    {
        case direction::north:
        {
            grid_pos new_pos = { pos.first - 1, pos.second };
            if (is_in_bounds(new_pos, grid) && grid[new_pos.first][new_pos.second] == '#')
                return true;
            break;
        }
        case direction::south:
        {
            grid_pos new_pos = { pos.first + 1, pos.second };
            if (is_in_bounds(new_pos, grid) && grid[new_pos.first][new_pos.second] == '#')
                return true;
            break;
        }
        case direction::east:
        {
            grid_pos new_pos = { pos.first, pos.second + 1 };
            if (is_in_bounds(new_pos, grid) && grid[new_pos.first][new_pos.second] == '#')
                return true;
            break;
        }
        case direction::west:
        {
            grid_pos new_pos = { pos.first, pos.second - 1 };
            if (is_in_bounds(new_pos, grid) && grid[new_pos.first][new_pos.second] == '#')
                return true;
            break;
        }
    }
    return false;
}

grid_pos step_forward(direction dir, const grid_pos& pos)
{
    switch (dir)
    {
        case direction::north: return { pos.first - 1, pos.second };
        case direction::south: return { pos.first + 1, pos.second };
        case direction::east: return { pos.first, pos.second + 1 };
        case direction::west: return { pos.first, pos.second - 1 };
    }
    throw std::runtime_error("something is wrong");
}

std::set<grid_pos> build_visited_map(const std::vector<std::string>& grid)
{
    std::set<grid_pos> visited;
    grid_pos curr_pos = get_starting_pos(grid);
    direction curr_direction = direction::north;

    while (is_in_bounds(curr_pos, grid))
    {
        while (check_collision(curr_direction, curr_pos, grid))
        {
            curr_direction = switch_direction(curr_direction);
        }

        if (visited.find(curr_pos) == visited.end())
        {
            visited.insert(curr_pos);
        }

        curr_pos = step_forward(curr_direction, curr_pos);
    }
    return visited;
}

void part_one(const std::vector<std::string>& grid)
{
    std::set<grid_pos> visited = build_visited_map(grid);
    std::cout << "[Part One] The final step count is " << visited.size() << '\n';
}

void part_two(std::vector<std::string>& grid)
{
    std::set<grid_pos> visited = build_visited_map(grid);
    const grid_pos starting_pos = get_starting_pos(grid);
    size_t valid_cycles = 0;

    // Now that our visited map is built, we want to check every step that the guard takes that if a '#'
    // is placed there will create a cycle
    for (int row = 0; row < grid.size(); ++row)
    {
        for (int col = 0; col < grid[row].size(); ++col)
        {
            if (row == starting_pos.first && col == starting_pos.second)
                continue;

            if (visited.find({ row, col }) != visited.end())
            {
                // This is a walkable step where we should try and place a '#'
                char c = grid[row][col];
                grid[row][col] = '#';

                grid_pos curr_pos = starting_pos;
                direction curr_direction = direction::north;
                std::set<std::pair<grid_pos, direction>> seen_states;
                seen_states.insert({ curr_pos, curr_direction });

                while (is_in_bounds(curr_pos, grid))
                {
                    while (check_collision(curr_direction, curr_pos, grid))
                    {
                        curr_direction = switch_direction(curr_direction);
                    }

                    curr_pos = step_forward(curr_direction, curr_pos);

                    auto state = std::make_pair(curr_pos, curr_direction);
                    if (!seen_states.insert(state).second)
                    {
                        ++valid_cycles;
                        break;
                    }
                }

                grid[row][col] = c;
            }
        }
    }

    std::cout << "[Part Two] The valid places to create a cycle count is " << valid_cycles << '\n';
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> contents = jumi::read_lines(file);
    part_one(contents);
    part_two(contents);
}
