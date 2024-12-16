#include "jumi_utils.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <optional>

enum class direction
{
    north, south, east, west
};

std::string direction_to_str(direction dir)
{
    switch (dir)
    {
        case direction::north: return "^";
        case direction::south: return "v";
        case direction::east: return ">";
        case direction::west: return "<";
    }
    return "";
}

struct coord
{
    int x;
    int y;
};

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<char>>& grid)
{
    for (const std::vector<char>& row : grid)
    {
        for (const char c : row)
        {
            std::cout << c;
        }
        std::cout << '\n';
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<direction>& directions)
{
    for (const direction& d : directions)
    {
        os << direction_to_str(d);
    }
    return os;
}

std::vector<std::vector<char>> get_grid(const std::vector<std::string>& contents)
{
    std::vector<std::vector<char>> grid;
    grid.resize(50);
    for (int i = 0; i < 50; ++i)
    {
        grid[i].resize(50);
    }

    for (int i = 0; i < 50; ++i)
    {
        for (int j = 0; j < 50; ++j)
        {
            grid[i][j] = contents[i][j];
        }
    }

    return grid;
}

std::vector<direction> get_direction_list(const std::vector<std::string>& contents)
{
    const int start_row = 51;
    std::vector<direction> directions;
    directions.reserve(512);

    for (int i = start_row; i < contents.size(); ++i)
    {
        for (int j = 0; j < contents[i].size(); ++j)
        {
            switch (contents[i][j])
            {
                case '^': directions.emplace_back(direction::north); break;
                case 'v': directions.emplace_back(direction::south); break;
                case '>': directions.emplace_back(direction::east); break;
                case '<': directions.emplace_back(direction::west); break;
            }
        }
    }

    return directions;
}

coord get_starting_location(const std::vector<std::vector<char>>& grid)
{
    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            if (grid[i][j] == '@')
            {
                return { j, i };
            }
        }
    }

    throw std::runtime_error("No valid starting location");
}

char check_collision(std::vector<std::vector<char>> grid, int x, int y, direction d)
{
    switch (d)
    {
        case direction::north:
            if (grid[y - 1][x] != '.') return grid[y - 1][x];
            break;
        case direction::south:
            if (grid[y + 1][x] != '.') return grid[y + 1][x];
            break;
        case direction::east:
            if (grid[y][x + 1] != '.') return grid[y][x + 1];
            break;
        case direction::west:
            if (grid[y][x - 1] != '.') return grid[y][x - 1];
            break;
    }

    return '.';
}

coord move_direction(std::vector<std::vector<char>>& grid, const coord& curr_coord, direction d)
{
    coord cached = curr_coord;
    coord new_coord = curr_coord;

    switch (d)
    {
        case direction::north:
            --new_coord.y;
            break;
        case direction::south:
            ++new_coord.y;
            break;
        case direction::east:
            ++new_coord.x;
            break;
        case direction::west:
            --new_coord.x;
            break;
    }
    grid[new_coord.y][new_coord.x] = '@';
    grid[cached.y][cached.x] = '.';

    return new_coord;
}

void draw_grid(int step, const std::vector<std::vector<char>>& grid, const coord& player_loc, direction d)
{
    system("clear");
    std::cout << "Step " << step << " - Next Direction: " << direction_to_str(d) << '\n';
    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            std::cout << grid[i][j];
        }

        if (i == player_loc.y)
        {
            std::cout << " <<< (Next Direction: " << direction_to_str(d) << ')';
        }

        std::cout << '\n';
    }

    for (int i = 0; i < player_loc.x; ++i)
        std::cout << ' ';
    std::cout << "^\n";
    for (int i = 0; i < player_loc.x; ++i)
        std::cout << ' ';
    std::cout << "^\n";
    for (int i = 0; i < player_loc.x; ++i)
        std::cout << ' ';
    std::cout << "^\n";
}

bool try_move_box(std::vector<std::vector<char>>& grid, const coord& curr_coord, direction d)
{
    static char box_char = 'O';
    coord box_location = curr_coord;
    coord initial_box_location = box_location;

    switch (d)
    {
        case direction::north:
        {
            box_location.y -= 1;
            initial_box_location = box_location;
            while (grid[box_location.y][box_location.x] == box_char)
            {
                box_location.y -=1;
            }
        } break;
        case direction::south:
        {
            box_location.y += 1;
            initial_box_location = box_location;
            while (grid[box_location.y][box_location.x] == box_char)
            {
                box_location.y +=1;
            }
        } break;
        case direction::east:
        {
            box_location.x += 1;
            initial_box_location = box_location;
            while (grid[box_location.y][box_location.x] == box_char)
            {
                box_location.x +=1;
            }
        } break;
        case direction::west:
        {
            box_location.x -= 1;
            initial_box_location = box_location;
            while (grid[box_location.y][box_location.x] == box_char)
            {
                box_location.x -=1;
            }
        } break;
    }

    const char final_char = grid[box_location.y][box_location.x];
    if (final_char == '.')
    {
        grid[initial_box_location.y][initial_box_location.x] = '.';
        grid[box_location.y][box_location.x] = box_char;
        return true;
    }
    return false;
}

void part_one(std::vector<std::vector<char>> grid, const std::vector<direction>& directions)
{
    coord current_loc = get_starting_location(grid);
    std::string input;
    int step = 0;

    for (const direction& d : directions)
    {
        // draw_grid(step, grid, current_loc, d);
        // std::getline(std::cin, input);

        // if (input == "q" || input == "quit")
        //     break;

        char collision_char = check_collision(grid, current_loc.x, current_loc.y, d);
        if (collision_char == '.')
        {
            current_loc = move_direction(grid, current_loc, d);
        }
        else if (collision_char == 'O')
        {
            bool player_can_move = try_move_box(grid, current_loc, d);

            if (player_can_move)
                current_loc = move_direction(grid, current_loc, d);
        }

        ++step;
    }

    size_t box_sum = 0;
    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            if (grid[i][j] == 'O')
            {
                box_sum += i * 100 + j;
            }
        }
    }

    std::cout << "[Part One] The total GPS sum for all boxes is " << box_sum << '\n';
}

int main()
{
    std::fstream file = jumi::open_file("input/day15_input.txt");
    std::vector<std::string> contents = jumi::read_lines(file);
    std::vector<std::vector<char>> grid = get_grid(contents);
    std::vector<direction> directions = get_direction_list(contents);
    part_one(grid, directions);
}
