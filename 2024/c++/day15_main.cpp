#include "jumi_utils.h"
#include <iostream>
#include <vector>
#include <string>

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

void part_one(std::vector<std::vector<char>> grid, const std::vector<direction>& directions)
{

}

int main()
{
    std::fstream file = jumi::open_file("input/day15_input.txt");
    std::vector<std::string> contents = jumi::read_lines(file);
    std::vector<std::vector<char>> grid = get_grid(contents);
    std::vector<direction> directions = get_direction_list(contents);
    part_one(grid, directions);
}
