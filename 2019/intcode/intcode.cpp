#include "intcode.h"
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

#define INT_MAX 2147483647

std::ostream& operator<<(std::ostream& os, const instruction& instr)
{
    os << "instruction={opcode=" << instr.opcode << ", lhs_loc=" <<
        instr.lhs_loc.value_or(-1) << ", rhs_loc=" << instr.rhs_loc.value_or(-1) << ", dest=" << instr.dest.value_or(-1) << "}";
    return os;
}

std::vector<int> get_opcodes(std::string& contents)
{
    std::vector<int> opcodes;

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

intcode::intcode(const std::vector<int>& instruction_stream)
    : _instructions(instruction_stream)
    , _ip()
    , _current_instruction()
{

}

void intcode::add()
{
    _current_instruction.lhs_loc = _instructions[_ip++];
    _current_instruction.rhs_loc = _instructions[_ip++];
    _current_instruction.dest = _instructions[_ip++];
    std::cout << "add instruction: " << _current_instruction;

    int lhs = _instructions[_current_instruction.lhs_loc.value()];
    int rhs = _instructions[_current_instruction.rhs_loc.value()];
    _instructions[_current_instruction.dest.value()] = lhs + rhs;
}

void intcode::mul()
{
    _current_instruction.lhs_loc = _instructions[_ip++];
    _current_instruction.rhs_loc = _instructions[_ip++];
    _current_instruction.dest = _instructions[_ip++];
    std::cout << "mul instruction: " << _current_instruction;

    int lhs = _instructions[_current_instruction.lhs_loc.value()];
    int rhs = _instructions[_current_instruction.rhs_loc.value()];
    _instructions[_current_instruction.dest.value()] = lhs * rhs;
}

void intcode::terminate()
{
    std::cout << "terminate instruction: " << _current_instruction;
    _ip = INT_MAX;
}

void intcode::execute_program()
{
    while (_ip < static_cast<int>(_instructions.size()))
    {
        _current_instruction = fetch_instruction();
        auto find_instruction = _opcode_map.find(_current_instruction.opcode);

        if (find_instruction != _opcode_map.end())
        {
            (this->*(find_instruction->second))();
        }

        std::cout << '\n';
    }
}

instruction intcode::fetch_instruction()
{
    if (_ip > static_cast<int>(_instructions.size()))
    {
        throw std::runtime_error("Instruction pointer cannot retrieve instruction past the end of the instruction stream");
    }

    instruction curr_instruction;
    curr_instruction.opcode = _instructions[_ip++];
    return curr_instruction;
}
