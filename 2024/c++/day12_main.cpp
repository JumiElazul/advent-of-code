#include "jumi_utils.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <fstream>
#include <string>
#include <optional>
#include <queue>
#include <unordered_set>

struct coord
{
    int x;
    int y;

    bool operator==(const coord& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool operator!=(const coord& rhs) const
    {
        return x != rhs.x && y != rhs.y;
    }
};

static coord directions[4] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };

struct coord_hash
{
    std::size_t operator()(const coord& c) const
    {
        return std::hash<int>()(c.x) ^ std::hash<int>()(c.y);
    }
};

struct region
{
    char c;
    int area;
    int perimeter;
    int sides;
    std::vector<coord> indices;

    bool empty() const { return area == 0 && perimeter == 0; }
};

std::ostream& operator<<(std::ostream& os, const region& r)
{
    os << "char: " << r.c << "   region perimeter: " << r.perimeter << "   region area: " << r.area;
    return os;
}

bool is_in_bounds(int x, int y, const std::vector<std::string>& grid)
{
    return x >= 0
        && y >= 0
        && x < grid[0].size()
        && y < grid.size();
}

void part_one(const std::vector<std::string>& grid)
{
    jumi::scoped_timer timer("Part One");
    std::unordered_set<coord, coord_hash> checked_indices;

    std::vector<region> regions;

    for (int row = 0; row < grid.size(); ++row)
    {
        for (int col = 0; col < grid[row].size(); ++col)
        {
            std::queue<coord> queue;
            queue.emplace(col, row);
            region r{};
            r.c = grid[row][col];

            while (!queue.empty())
            {
                coord curr = queue.front();
                queue.pop();

                if (checked_indices.find(curr) != checked_indices.end())
                    continue;

                checked_indices.emplace(curr);
                ++r.area;
                r.indices.emplace_back(curr);

                const char curr_char = grid[curr.y][curr.x];
                for (int i = 0; i < 4; ++i)
                {
                    coord next_coord = { curr.x + directions[i].x, curr.y + directions[i].y };
                    if (is_in_bounds(next_coord.x, next_coord.y, grid) && curr_char == grid[next_coord.y][next_coord.x])
                    {
                        queue.emplace(next_coord);
                    }
                    else
                    {
                        ++r.perimeter;
                    }
                }
            }

            if (!r.empty())
            {
                regions.push_back(r);
            }
        }
    }

    size_t result = 0;
    for (const region& r : regions)
    {
        std::cout << r << '\n';
        result += (r.area * r.perimeter);
    }

    std::cout << "[Part One] The total price of all fences is " << result << '\n';
}

void part_two(const std::vector<std::string>& grid)
{
    jumi::scoped_timer timer("Part Two");

    std::unordered_set<coord, coord_hash> checked_indices;

    std::vector<region> regions;

    for (int row = 0; row < grid.size(); ++row)
    {
        for (int col = 0; col < grid[row].size(); ++col)
        {
            std::queue<coord> queue;
            queue.emplace(col, row);
            region r{};
            r.c = grid[row][col];

            while (!queue.empty())
            {
                coord curr = queue.front();
                queue.pop();

                if (checked_indices.find(curr) != checked_indices.end())
                    continue;

                checked_indices.emplace(curr);
                ++r.area;
                r.indices.emplace_back(curr);

                const char curr_char = grid[curr.y][curr.x];
                for (int i = 0; i < 4; ++i)
                {
                    coord next_coord = { curr.x + directions[i].x, curr.y + directions[i].y };
                    if (is_in_bounds(next_coord.x, next_coord.y, grid) && curr_char == grid[next_coord.y][next_coord.x])
                    {
                        queue.emplace(next_coord);
                    }
                    else
                    {
                        ++r.perimeter;
                    }
                }
            }

            if (!r.empty())
            {
                regions.push_back(r);
            }
        }
    }

    size_t result = 0;
    for (const region& r : regions)
    {
        result += (r.area * r.sides);
    }

    std::cout << "[Part Two] The total price of all fences is " << result << '\n';
}

int main()
{
    std::fstream file = jumi::open_file("input/day12_input.txt");
    std::vector<std::string> grid = jumi::read_lines(file);
    std::vector<std::string> test =
    {
        "AAAA",
        "BBCD",
        "BBCC",
        "EEEC",
    };
    part_one(grid);
    part_two(grid);
}
