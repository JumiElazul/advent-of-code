#include "jumi_utils.h"
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>

using opcode = int;
std::vector<opcode> get_opcodes(std::string& contents)
{
    std::vector<opcode> opcodes;

    char delimiter = ',';
    size_t current_pos = 0;
    size_t end_pos = 0;

    while ((end_pos = contents.find(delimiter, current_pos)) != std::string::npos)
    {
        opcodes.push_back(std::stoi(contents.substr(current_pos, end_pos)));
        current_pos = end_pos + 1;
    }

    opcodes.push_back(std::stoi(contents.substr(current_pos, end_pos)));

    return opcodes;
}

void part_one(std::vector<opcode> opcodes)
{
    opcodes[1] = 12;
    opcodes[2] = 2;
    bool end_program = false;

    for (size_t i = 0; i < opcodes.size(); i += 4)
    {
        char operator_;
        switch (opcodes[i])
        {
            case 1: operator_ = '+'; break;
            case 2: operator_ = '*'; break;
            case 99: end_program = true; break;
        }
        if (end_program) break;

        int lhs = opcodes[opcodes[i + 1]];
        int rhs = opcodes[opcodes[i + 2]];

        if
            (operator_ == '+') opcodes[opcodes[i + 3]] = lhs + rhs;
        else if
            (operator_ == '*') opcodes[opcodes[i + 3]] = lhs * rhs;
    }

    std::cout << "[Part One] The final value at position 0 is: " << opcodes[0] << '\n';
}

void part_two(std::vector<opcode> opcodes_)
{
    int target = 19690720;
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 100; ++j)
        {
            std::vector<opcode> opcodes = opcodes_;
            opcodes[1] = i;
            opcodes[2] = j;

            bool end_program = false;

            for (size_t i = 0; i < opcodes.size(); i += 4)
            {
                char operator_;
                switch (opcodes[i])
                {
                    case 1: operator_ = '+'; break;
                    case 2: operator_ = '*'; break;
                    case 99: end_program = true; break;
                    default: end_program = true; break;
                }
                if (end_program) break;

                int lhs = opcodes[opcodes[i + 1]];
                int rhs = opcodes[opcodes[i + 2]];

                if
                    (operator_ == '+') opcodes[opcodes[i + 3]] = lhs + rhs;
                else if
                    (operator_ == '*') opcodes[opcodes[i + 3]] = lhs * rhs;
            }

            if (opcodes[0] == target)
            {
                std::cout << "[Part Two] i (" << i << ") and j (" << j << ") result in " << target
                    << ", final result is " << 100 * i + j << '\n';
                return;
            }
        }
    }

}

int main()
{
    std::fstream file = jumi::open_file("input/day2_input.txt");
    std::string contents = jumi::stringify_file(file);
    std::vector<opcode> opcodes = get_opcodes(contents);
    part_one(opcodes);
    part_two(opcodes);
}

