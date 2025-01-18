#include "intcode.h"
#include <string>
#include <iostream>
#include <optional>

#define INT_MIN -2147483648
#define INT_MAX 2147483647

std::string memory_mode_to_string(memory_mode mode)
{
    switch (mode)
    {
        case memory_mode::position:
            return "position";
        case memory_mode::immediate:
            return "immediate";
        default:
            return "unknown";
    }
}

std::ostream& operator<<(std::ostream& os, const instruction& instr)
{
    os << "instruction={raw_opcode=" << instr.raw_opcode << ", opcode=" << instr.opcode << ", parameter_memory_modes=[";
    for (size_t i = 0; i < instr.parameter_memory_modes.size(); ++i)
    {
        os << memory_mode_to_string(instr.parameter_memory_modes[i]);
        if (i != instr.parameter_memory_modes.size() - 1)
            os << ", ";
    }
    os << "]";

    if (instr.param1.has_value()) os << ", param1=" << instr.param1.value();
    if (instr.param2.has_value()) os << ", param2=" << instr.param2.value();
    if (instr.param3.has_value()) os << ", param3=" << instr.param3.value();

    os << "}";
    return os;
}

intcode::intcode(const std::string& instruction_stream) 
    : _instructions(get_opcodes(instruction_stream))
    , _ip()
    , _current_instruction()
{

}

void intcode::execute_program()
{
    while (_ip < static_cast<int>(_instructions.size()))
    {
        _current_instruction = fetch_instruction();
        auto find_instruction = _opcode_map.find(_current_instruction.opcode);

        if (find_instruction != _opcode_map.end())
        {
            std::cout << find_instruction->second.name << ": " << _current_instruction << '\n';
            (this->*(find_instruction->second.func))();
        }
    }
}

void intcode::add()
{
    fetch_params(3);
    memory_mode lhs_mode = _current_instruction.parameter_memory_modes[0];
    memory_mode rhs_mode = _current_instruction.parameter_memory_modes[1];
    int lhs = decode_memory_mode(_current_instruction.param1, lhs_mode);
    int rhs = decode_memory_mode(_current_instruction.param2, rhs_mode);

    _instructions[_current_instruction.param3.value()] = lhs + rhs;
}

void intcode::mul()
{
    fetch_params(3);
    memory_mode lhs_mode = _current_instruction.parameter_memory_modes[0];
    memory_mode rhs_mode = _current_instruction.parameter_memory_modes[1];
    int lhs = decode_memory_mode(_current_instruction.param1, lhs_mode);
    int rhs = decode_memory_mode(_current_instruction.param2, rhs_mode);

    _instructions[_current_instruction.param3.value()] = lhs * rhs;
}

void intcode::inp()
{
    fetch_params(1);
    intcode_std_out("intcode > ", false);
    std::string input;
    std::getline(std::cin, input);
    // Hard coded because input values will always use positional (memory) mode
    _instructions[_current_instruction.param1.value()] = std::stoi(input);
}

void intcode::out()
{
    fetch_params(1);
    memory_mode mode = _current_instruction.parameter_memory_modes[0];
    int output = decode_memory_mode(_current_instruction.param1, mode);

    intcode_std_out(std::to_string(output));
}

void intcode::jump_if_true()
{
    fetch_params(2);
    memory_mode lhs_mode = _current_instruction.parameter_memory_modes[0];
    memory_mode rhs_mode = _current_instruction.parameter_memory_modes[1];
    int lhs = decode_memory_mode(_current_instruction.param1, lhs_mode);
    int rhs = decode_memory_mode(_current_instruction.param2, rhs_mode);

    if (lhs != 0) _ip = rhs;
}

void intcode::jump_if_false()
{
    fetch_params(2);
    memory_mode lhs_mode = _current_instruction.parameter_memory_modes[0];
    memory_mode rhs_mode = _current_instruction.parameter_memory_modes[1];
    int lhs = decode_memory_mode(_current_instruction.param1, lhs_mode);
    int rhs = decode_memory_mode(_current_instruction.param2, rhs_mode);

    if (lhs == 0) _ip = rhs;
}

void intcode::less_than()
{
    fetch_params(3);
    memory_mode lhs_mode = _current_instruction.parameter_memory_modes[0];
    memory_mode rhs_mode = _current_instruction.parameter_memory_modes[1];
    int lhs = decode_memory_mode(_current_instruction.param1, lhs_mode);
    int rhs = decode_memory_mode(_current_instruction.param2, rhs_mode);
    (lhs < rhs) 
        ? _instructions[_current_instruction.param3.value()] = 1
        : _instructions[_current_instruction.param3.value()] = 0;
}

void intcode::equals()
{
    fetch_params(3);
    memory_mode lhs_mode = _current_instruction.parameter_memory_modes[0];
    memory_mode rhs_mode = _current_instruction.parameter_memory_modes[1];
    int lhs = decode_memory_mode(_current_instruction.param1, lhs_mode);
    int rhs = decode_memory_mode(_current_instruction.param2, rhs_mode);
    (lhs == rhs) 
        ? _instructions[_current_instruction.param3.value()] = 1
        : _instructions[_current_instruction.param3.value()] = 0;
}

void intcode::terminate()
{
    _ip = INT_MAX;
}

instruction intcode::fetch_instruction()
{
    int instr = fetch_int();

    int raw_opcode = instr;
    int opcode = instr % 100;
    int param_mode1 = (instr / 100)   % 10;
    int param_mode2 = (instr / 1000)  % 10;
    int param_mode3 = (instr / 10000) % 10;

    instruction curr_instruction;
    curr_instruction.raw_opcode = raw_opcode;
    curr_instruction.opcode = opcode;
    curr_instruction.parameter_memory_modes.push_back(static_cast<memory_mode>(param_mode1));
    curr_instruction.parameter_memory_modes.push_back(static_cast<memory_mode>(param_mode2));
    curr_instruction.parameter_memory_modes.push_back(static_cast<memory_mode>(param_mode3));

    return curr_instruction;
}

int intcode::fetch_int()
{
    if (_ip >= static_cast<int>(_instructions.size()))
    {
        throw std::runtime_error("Instruction pointer cannot retrieve instruction past the end of the instruction stream");
    }

    int param = _instructions[_ip++];
    return param;
}

void intcode::fetch_params(int num_params)
{
    if (num_params < 1 || num_params > 3)
    {
        throw std::runtime_error("Invalid number of parameters to fetch");
    }

    if (num_params >= 1) _current_instruction.param1 = fetch_int();
    if (num_params >= 2) _current_instruction.param2 = fetch_int();
    if (num_params >= 3) _current_instruction.param3 = fetch_int();
}

int intcode::decode_memory_mode(const std::optional<int>& value, memory_mode mode)
{
    switch (mode)
    {
        case memory_mode::position:
        {
            return _instructions[value.value()];
        } break;
        case memory_mode::immediate:
        {
            return value.value();
        } break;
        default:
        {
            return INT_MIN;
        } break;
    }
}

void intcode::intcode_std_out(const std::string& output, bool newline)
{
    std::cout << output;
    if (newline) std::cout << '\n';
}

std::vector<int> intcode::get_opcodes(std::string instruction_stream)
{
    std::vector<int> opcodes;

    char delimiter = ',';
    size_t current_pos = 0;
    size_t end_pos = 0;

    while ((end_pos = instruction_stream.find(delimiter, current_pos)) != std::string::npos)
    {
        opcodes.push_back(std::stoi(instruction_stream.substr(current_pos, end_pos)));
        current_pos = end_pos + 1;
    }

    opcodes.push_back(std::stoi(instruction_stream.substr(current_pos, end_pos)));

    return opcodes;
}
