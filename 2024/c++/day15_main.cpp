#include "jumi_utils.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

enum class direction { north, south, east, west };
static const char BOX_CHAR = 'O';

struct coord
{
    int x;
    int y;
};

struct direction_info
{
    direction dir;
    int dx;
    int dy;
    char symbol;
};

static const direction_info DIRECTION_INFO[]
{
    { direction::north, 0, -1, '^' },
    { direction::south, 0,  1, 'v' },
    { direction::east,  1,  0, '>' },
    { direction::west, -1,  0, '<' },
};

std::string direction_to_str(direction dir)
{
    for (auto& info : DIRECTION_INFO)
        if (info.dir == dir) return std::string(1, info.symbol);

    return "";
}

std::pair<int, int> get_direction_offset(direction d)
{
    for (auto& info : DIRECTION_INFO)
    {
        if (info.dir == d) return { info.dx, info.dy };
    }
    return { 0, 0 };
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
    auto [dx, dy] = get_direction_offset(d);
    int nx = x + dx;
    int ny = y + dy;

    if (nx < 0 || nx >= grid[0].size() || ny < 0 || ny >= grid.size())
        return '#';

    return grid[ny][nx];
}

coord move_player(std::vector<std::vector<char>>& grid, const coord& curr_coord, direction d)
{
    auto [dx, dy] = get_direction_offset(d);
    coord new_coord = { curr_coord.x + dx, curr_coord.y + dy };
    grid[new_coord.y][new_coord.x] = '@';
    grid[curr_coord.y][curr_coord.x] = '.';
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
    auto [dx, dy] = get_direction_offset(d);

    coord check_pos = { curr_coord.x + dx, curr_coord.y + dy };
    coord last_box_pos = check_pos;

    while (grid[last_box_pos.y][last_box_pos.x] == BOX_CHAR)
    {
        last_box_pos.x += dx;
        last_box_pos.y += dy;

        if (last_box_pos.x < 0 || last_box_pos.x >= grid[0].size()  ||
            last_box_pos.y < 0 || last_box_pos.y >= grid.size()     ||
            (grid[last_box_pos.y][last_box_pos.x] != '.' && grid[last_box_pos.y][last_box_pos.x] != BOX_CHAR)) 
        {
            return false;
        }
    }

    if (grid[last_box_pos.y][last_box_pos.x] == '.') 
    {
        while (last_box_pos.x != curr_coord.x + dx || last_box_pos.y != curr_coord.y + dy) 
        {
            coord prev = { last_box_pos.x - dx, last_box_pos.y - dy };
            grid[last_box_pos.y][last_box_pos.x] = BOX_CHAR;
            last_box_pos = prev;
        }

        grid[last_box_pos.y][last_box_pos.x] = '.';
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
            current_loc = move_player(grid, current_loc, d);
        }
        else if (collision_char == BOX_CHAR)
        {
            if (try_move_box(grid, current_loc, d))
            {
                current_loc = move_player(grid, current_loc, d);
            }
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
