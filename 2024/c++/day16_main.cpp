#include "jumi_utils.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>

struct coord
{
    int x;
    int y;
};

std::pair<coord, coord> get_start_and_end_location(const std::vector<std::string>& grid)
{
    std::pair<coord, coord> result;

    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            if (grid[i][j] == 'S')
                result.first = coord{ j, i };
            else if (grid[i][j] == 'S')
                result.second = coord{ j, i };
        }
    }

    if ((result.first.x == 0 && result.first.y == 0) ||(result.second.x == 0 && result.second.y == 0))
        throw std::runtime_error("Either a valid start or end location was not found");

    return result;
}

void part_one(const std::vector<std::string>& grid)
{
    auto [start, end] = get_start_and_end_location(grid);
}

int main()
{
    std::fstream file = jumi::open_file("input/day16_input.txt");
    std::vector<std::string> contents = jumi::read_lines(file);
    // part_one(contents);
    std::pair<coord, coord> my_pair;
    std::cout << my_pair.first.x << ':' << my_pair.first.y << "  -  " << my_pair.second.x << ':' << my_pair.second.y << '\n';
}
