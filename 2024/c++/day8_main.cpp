#include "jumi_utils.h"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>

constexpr const char* filepath = "input/day8_input.txt";
std::fstream file = jumi::open_file(filepath);
std::vector<std::string> grid = jumi::read_lines(file);

struct antenna_location
{
    int x;
    int y;

    bool operator==(const antenna_location& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
};

struct antinode_location
{
    int x;
    int y;

    bool operator==(const antinode_location& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
};

struct antinode_hash
{
    std::size_t operator()(const antinode_location& antinode) const
    {
        return std::hash<int>{}(antinode.x) ^ std::hash<int>{}(antinode.y);
    }
};

bool is_in_bounds(int x, int y, const std::vector<std::string>& grid)
{
    return x >= 0
        && y >= 0
        && x < grid[0].size()
        && y < grid.size();
}

std::unordered_map<char, std::vector<antenna_location>> get_antenna_locations(const std::vector<std::string>& grid)
{
    std::unordered_map<char, std::vector<antenna_location>> locations;
    for (int row = 0; row < grid.size(); ++row)
    {
        for (int col = 0; col < grid[row].size(); ++col)
        {
            if (std::isalnum(grid[row][col]))
            {
                locations[grid[row][col]].emplace_back(row, col);
            }
        }
    }
    return locations;
}

void part_one(const std::unordered_map<char, std::vector<antenna_location>>& antenna_locations)
{
    std::vector<antinode_location> antinodes;

    for (const auto& kvp : antenna_locations)
    {
        const std::vector<antenna_location>& locations = kvp.second;
        for (int i = 0; i < kvp.second.size(); ++i)
        {
            for (int j = i + 1; j < kvp.second.size(); ++j)
            {
                const antenna_location& A = locations[i];
                const antenna_location& B = locations[j];

                int dx = A.x - B.x;
                int dy = A.y - B.y;

                antinode_location antinode1 { A.x + dx, A.y + dy };
                antinode_location antinode2 { B.x - dx, B.y - dy };

                if (is_in_bounds(antinode1.x, antinode1.y, grid))
                {
                    antinodes.push_back(antinode1);
                }
                if (is_in_bounds(antinode2.x, antinode2.y, grid))
                {
                    antinodes.push_back(antinode2);
                }
            }
        }
    }

    std::unordered_set<antinode_location, antinode_hash> unique_antinodes(antinodes.begin(), antinodes.end());
    std::cout << "[Part One] The total count of all antinodes is " << unique_antinodes.size() << '\n';
}

void part_two(const std::unordered_map<char, std::vector<antenna_location>>& antenna_locations)
{
    std::vector<antinode_location> antinodes;

    for (const auto& kvp : antenna_locations)
    {
        const std::vector<antenna_location>& locations = kvp.second;
        for (const auto& loc : locations)
        {
            antinodes.emplace_back(loc.x, loc.y);
        }

        for (int i = 0; i < kvp.second.size(); ++i)
        {
            for (int j = i + 1; j < kvp.second.size(); ++j)
            {
                const antenna_location& A = locations[i];
                const antenna_location& B = locations[j];

                int dx = A.x - B.x;
                int dy = A.y - B.y;

                antinode_location antinode1 { A.x + dx, A.y + dy };
                antinode_location antinode2 { B.x - dx, B.y - dy };

                while (is_in_bounds(antinode1.x, antinode1.y, grid))
                {
                    antinodes.push_back(antinode1);
                    antinode1 = { antinode1.x + dx, antinode1.y + dy };
                }
                while (is_in_bounds(antinode2.x, antinode2.y, grid))
                {
                    antinodes.push_back(antinode2);
                    antinode2 = { antinode2.x - dx, antinode2.y - dy };
                }
            }
        }
    }

    std::unordered_set<antinode_location, antinode_hash> unique_antinodes(antinodes.begin(), antinodes.end());
    std::cout << "[Part Two] The total count of all antinodes is " << unique_antinodes.size() << '\n';
}

int main()
{
    std::unordered_map<char, std::vector<antenna_location>> antenna_locations = get_antenna_locations(grid);
    part_one(antenna_locations);
    part_two(antenna_locations);
}
