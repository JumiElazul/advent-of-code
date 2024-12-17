#include "jumi_utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

struct cpu_register
{
    int a;
    int b;
    int c;
};

class cpu
{
public:
    cpu(const std::vector<unsigned char>& instructions)
        : _registers{ 0, 0, 0 }
        , _instructions{ std::move(instructions) }
        , _ip(0)
    {

    }

    void increment_ip(int value = 2)
    {
        _ip += value;
    }

private:
    cpu_register _registers;
    std::vector<unsigned char> _instructions;
    int _ip;
};

std::vector<unsigned char> get_instructions(const std::vector<std::string>& contents)
{
    std::vector<unsigned char> instructions;
    std::string target = "Program: ";

    for (const std::string& line : contents)
    {
        if (line.find(target) != std::string::npos)
        {
            std::string substr = line.substr(target.size());
            std::stringstream ss(substr);
            std::string token;
            while (std::getline(ss, token, ','))
            {
                instructions.push_back(std::stoi(token));
            }
        }
    }

    return instructions;
}

int main()
{
    std::fstream file = jumi::open_file("input/day17_input.txt");
    std::vector<std::string> contents = jumi::read_lines(file);
    std::vector<unsigned char> instructions = get_instructions(contents);
}
