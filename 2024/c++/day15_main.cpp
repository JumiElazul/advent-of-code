#include "jumi_utils.h"
#include <algorithm>
#include <queue>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

enum class direction { north, south, east, west };
static const char P1_BOX_CHAR = 'O';
static const char P2_BOX_CHARS[] = { '[', ']' };

static bool step_through_code = false;

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
    directions.reserve(4096);

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
    std::cout << "Step " << step << " - Next Direction: " << direction_to_str(d) << "( 's' or 'skip' to skip rest )" << '\n';
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
    coord last_box_pos = { curr_coord.x + dx, curr_coord.y + dy };

    while (grid[last_box_pos.y][last_box_pos.x] == P1_BOX_CHAR)
    {
        last_box_pos.x += dx;
        last_box_pos.y += dy;

        if (last_box_pos.x < 0 || last_box_pos.x >= grid[0].size()  ||
            last_box_pos.y < 0 || last_box_pos.y >= grid.size()     ||
            (grid[last_box_pos.y][last_box_pos.x] != '.' && grid[last_box_pos.y][last_box_pos.x] != P1_BOX_CHAR)) 
        {
            return false;
        }
    }

    if (grid[last_box_pos.y][last_box_pos.x] == '.') 
    {
        while (last_box_pos.x != curr_coord.x + dx || last_box_pos.y != curr_coord.y + dy) 
        {
            coord prev = { last_box_pos.x - dx, last_box_pos.y - dy };
            grid[last_box_pos.y][last_box_pos.x] = P1_BOX_CHAR;
            last_box_pos = prev;
        }

        grid[last_box_pos.y][last_box_pos.x] = '.';
        return true;
    }

    return false;
}

bool try_move_box_part_two(std::vector<std::vector<char>>& grid, const coord& curr_coord, direction d)
{
    auto [dx, dy] = get_direction_offset(d);
    coord current_pos = { curr_coord.x + dx, curr_coord.y + dy };

    if (d == direction::west || d == direction::east)
    {
        std::vector<coord> box_positions;

        while (std::find(std::begin(P2_BOX_CHARS), std::end(P2_BOX_CHARS), grid[current_pos.y][current_pos.x]) != std::end(P2_BOX_CHARS))
        {
            if (current_pos.x < 0 || current_pos.x >= grid[0].size() || current_pos.y < 0 || current_pos.y >= grid.size())
            {
                return false;
            }

            box_positions.push_back(current_pos);

            current_pos.x += dx;
            current_pos.y += dy;
        }

        char next_cell_after_boxes = grid[current_pos.y][current_pos.x];
        if (next_cell_after_boxes == '.')
        {
            for (int i = static_cast<int>(box_positions.size()) - 1; i >= 0; --i)
            {
                coord old_pos = box_positions[i];
                coord new_pos = { old_pos.x + dx, old_pos.y + dy };

                grid[new_pos.y][new_pos.x] = grid[old_pos.y][old_pos.x];
                grid[old_pos.y][old_pos.x] = '.';
            }
            return true;
        }
    }
    else
    {
        std::queue<coord> queue;
        std::vector<std::pair<coord, char>> box_positions;

        auto enqueue_pair = [&](const coord& c1, const coord& c2) {
            queue.emplace(c1);
            queue.emplace(c2);

            box_positions.emplace_back(c1, grid[c1.y][c1.x]);
            box_positions.emplace_back(c2, grid[c2.y][c2.x]);
        };

        char front_char = grid[current_pos.y][current_pos.x];
        if (front_char == '[')
        {
            enqueue_pair(current_pos, coord{ current_pos.x + 1, current_pos.y });
        }
        else if (front_char == ']')
        {
            enqueue_pair(coord{ current_pos.x - 1, current_pos.y }, current_pos);
        }

        while (!queue.empty())
        {
            if (queue.size() < 2)
            {
                throw std::runtime_error("Something is wrong with the box chain logic.");
            }

            int pairs_to_process = static_cast<int>(queue.size()) / 2;

            for (int i = 0; i < pairs_to_process; ++i)
            {
                coord left_half = queue.front();  queue.pop();
                coord right_half = queue.front(); queue.pop();

                char left_next  = grid[left_half.y + dy][left_half.x + dx];
                char right_next = grid[right_half.y + dy][right_half.x + dx];

                if (left_next == '[')
                {
                    enqueue_pair(coord{left_half.x + dx, left_half.y + dy},
                            coord{left_half.x + dx + 1, left_half.y + dy});
                }
                else if (left_next == ']')
                {
                    enqueue_pair(coord{left_half.x + dx - 1, left_half.y + dy},
                            coord{left_half.x + dx, left_half.y + dy});
                }
                else if (left_next == '#')
                {
                    return false;
                }

                if (right_next == '[')
                {
                    enqueue_pair(coord{right_half.x + dx, right_half.y + dy},
                            coord{right_half.x + dx + 1, right_half.y + dy});
                }
                else if (right_next == ']')
                {
                    enqueue_pair(coord{right_half.x + dx - 1, right_half.y + dy},
                            coord{right_half.x + dx, right_half.y + dy});
                }
                else if (right_next == '#')
                {
                    return false;
                }
            }
        }

        for (auto& p : box_positions)
        {
            const coord& c = p.first;
            grid[c.y][c.x] = '.'; 
        }

        for (auto& p : box_positions)
        {
            const coord& old_c = p.first;
            const char ch = p.second;
            coord new_pos = { old_c.x + dx, old_c.y + dy };
            grid[new_pos.y][new_pos.x] = ch;
        }

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
        if (step_through_code)
        {
            draw_grid(step, grid, current_loc, d);
            std::getline(std::cin, input);

            if (input == "q" || input == "quit")
                break;
            else if (input == "s" || input == "skip")
                step_through_code = false;
        }

        char collision_char = check_collision(grid, current_loc.x, current_loc.y, d);

        if (collision_char == '.')
        {
            current_loc = move_player(grid, current_loc, d);
        }
        else if (collision_char == P1_BOX_CHAR)
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

    step_through_code = false;
    std::cout << "[Part One] The total GPS sum for all boxes is " << box_sum << '\n';
}

std::vector<std::vector<char>> modify_grid(const std::vector<std::vector<char>>& grid)
{
    std::vector<std::vector<char>> new_grid;
    new_grid.resize(grid.size(), std::vector<char>());

    for (int i = 0; i < new_grid.size(); ++i)
    {
        new_grid[i] = std::vector<char>(grid.size() * 2, '.');
    }

    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            int new_j = 2 * j;

            const char curr_char = grid[i][j];
            if (curr_char == '#')
            {
                new_grid[i][new_j] = '#';
                new_grid[i][new_j + 1] = '#';
            }
            else if (curr_char == 'O')
            {
                new_grid[i][new_j] = '[';
                new_grid[i][new_j + 1] = ']';
            }
            else if (curr_char == '.')
            {
                new_grid[i][new_j] = '.';
                new_grid[i][new_j + 1] = '.';
            }
            else if (curr_char == '@')
            {
                new_grid[i][new_j] = '@';
                new_grid[i][new_j + 1] = '.';
            }
        }
    }

    return new_grid;
}

void part_two(std::vector<std::vector<char>>& grid, const std::vector<direction>& directions)
{
    grid = modify_grid(grid);
    coord current_loc = get_starting_location(grid);

    std::string input;
    int step = 0;

    for (int i = 0; i < directions.size(); ++i)
    {
        if (step_through_code)
        {
            draw_grid(step, grid, current_loc, directions[i]);
            std::getline(std::cin, input);

            if (input == "q" || input == "quit")
                break;
            else if (input == "s" || input == "skip")
                step_through_code = false;
        }

        char collision_char = check_collision(grid, current_loc.x, current_loc.y, directions[i]);

        if (collision_char == '.')
        {
            current_loc = move_player(grid, current_loc, directions[i]);
        }
        else if (std::find(std::begin(P2_BOX_CHARS), std::end(P2_BOX_CHARS), collision_char) != std::end(P2_BOX_CHARS))
        {
            if (try_move_box_part_two(grid, current_loc, directions[i]))
            {
                current_loc = move_player(grid, current_loc, directions[i]);
            }
        }

        ++step;
    }

    size_t box_sum = 0;
    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            if (grid[i][j] == '[')
            {
                box_sum += i * 100 + j;
            }
        }
    }

    std::cout << "[Part Two] The total GPS sum for all boxes is " << box_sum << '\n';
}

int main()
{
    std::fstream file = jumi::open_file("input/day15_input.txt");
    std::vector<std::string> contents = jumi::read_lines(file);
    std::vector<std::vector<char>> grid = get_grid(contents);
    std::vector<direction> directions = get_direction_list(contents);

    std::cout << "Would you like to step through part one? (y/n): ";
    std::string input;
    std::getline(std::cin, input);

    if (input == "y" || input == "yes")
        step_through_code = true;

    part_one(grid, directions);

    std::cout << "Would you like to step through part two? (y/n): ";
    std::getline(std::cin, input);

    if (input == "y" || input == "yes")
        step_through_code = true;

    part_two(grid, directions);
}
