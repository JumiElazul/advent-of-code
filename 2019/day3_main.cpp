#include "jumi_utils.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_set>

struct coord
{
    int x;
    int y;
};

struct coord_hash
{
    std::size_t operator()(const coord& c) const
    {
        return std::hash<int>()(c.x) ^ std::hash<int>()(c.y);
    }
};

void execute_instruction(const std::string& instruction, std::unordered_map<coord, coord_hash>& uset)
{
    int instr = std::stoi(instruction.substr(1));
    switch (instruction[0])
    {
        case 'U': 
            break;
        case 'D': 
            break;
        case 'L': 
            break;
        case 'R': 
            break;
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
        i == 0 ? execute_instruction(line1_visited) : execute_instruction(line2_visited);
    }
}

int main()
{
    std::fstream file = jumi::open_file("input/day3_input.txt");
    std::vector<std::string> contents = jumi::read_lines(file);
}
