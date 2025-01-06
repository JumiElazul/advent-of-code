#include "jumi_utils.h"
#include <algorithm>
#include <climits>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_set>

struct coord
{
    int x;
    int y;

    bool operator==(const coord& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct coord_hash
{
    std::size_t operator()(const coord& c) const
    {
        return std::hash<int>()(c.x) ^ std::hash<int>()(c.y);
    }
};

void execute_instruction(coord& curr_pos, const std::string& instruction, std::unordered_set<coord, coord_hash>& uset)
{
    int instr = std::stoi(instruction.substr(1));
    switch (instruction[0])
    {
        case 'U': 
        {
            for (int i = 0; i < instr; ++i)
            {
                curr_pos.y -= 1;
                uset.insert(curr_pos);
            }
        } break;
        case 'D': 
        {
            for (int i = 0; i < instr; ++i)
            {
                curr_pos.y += 1;
                uset.insert(curr_pos);
            }
        } break;
        case 'L': 
        {
            for (int i = 0; i < instr; ++i)
            {
                curr_pos.x -= 1;
                uset.insert(curr_pos);
            }
        } break;
        case 'R': 
        {
            for (int i = 0; i < instr; ++i)
            {
                curr_pos.x += 1;
                uset.insert(curr_pos);
            }
        } break;
    }
}

void part_one(const std::vector<std::string>& lines)
{
    std::unordered_set<coord, coord_hash> line1_visited;
    std::unordered_set<coord, coord_hash> line2_visited;

    for (int i = 0; i < lines.size(); ++i)
    {
        coord start_pos = { 0, 0 };
        std::vector<std::string> instructions = jumi::split(lines[i], ',');
        for (int j = 0; j < instructions.size(); ++j)
        {
            const std::string& instr = instructions[j];
            i == 0 ? execute_instruction(start_pos, instr, line1_visited)
                   : execute_instruction(start_pos, instr, line2_visited);
        }
    }

    std::unordered_set<coord, coord_hash> intersection;
    for (const coord& c : line1_visited)
    {
        if (line2_visited.find(c) != line2_visited.end())
        {
            intersection.insert(c);
        }
    }

    int min_distance = INT_MAX;

    for (const coord& c : intersection)
    {
        int dist = std::abs(c.x) + std::abs(c.y);
        if (dist < min_distance)
        {
            min_distance = dist;
        }
    }

    std::cout << "[Part One] The mininmum distance for an intersection is: " << min_distance << '\n';
}

int main()
{
    std::fstream file = jumi::open_file("input/day3_input.txt");
    std::vector<std::string> contents = jumi::read_lines(file);
    part_one(contents);
}
