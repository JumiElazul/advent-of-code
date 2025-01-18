#include "intcode.h"
#include <array>
#include <string>
#include <iostream>

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
    os << "instruction {\n"
       << "  raw_opcode: " << instr.raw_opcode << ",\n"
       << "  opcode: " << instr.opcode << ",\n"
       << "  param_count: " << instr.param_count << ",\n"
       << "  params: [\n";

    for (int i = 0; i < instr.param_count; ++i)
    {
        os << "    { value: " 
           << instr.params[i].value 
           << ", mode: " 
           << memory_mode_to_string(instr.params[i].mode)
           << " }";
        if (i < instr.param_count - 1)
            os << ","; 
        os << "\n";
    }
    
    os << "  ]\n"
       << "}";

    return os;
}

intcode::intcode(const std::string& instruction_stream) 
    : _instructions(get_opcodes(instruction_stream))
    , _ip()
    , _curr_instr()
{

}

void intcode::execute_program()
{
    while (_ip < static_cast<int>(_instructions.size()))
    {
        _curr_instr = fetch_instruction();
        auto find_instruction = _opcode_map.find(_curr_instr.opcode);

        if (find_instruction != _opcode_map.end())
        {
#ifdef PRINT_OPCODES
            std::cout << find_instruction->second.name << ": " << _curr_instr << '\n';
#endif
            (this->*(find_instruction->second.func))();
        }
    }
}

void intcode::add()
{
    int lhs = read_param(_curr_instr, 0);
    int rhs = read_param(_curr_instr, 1);
    int dest = write_param(_curr_instr, 2);

    _instructions[dest] = lhs + rhs;
}

void intcode::mul()
{
    int lhs = read_param(_curr_instr, 0);
    int rhs = read_param(_curr_instr, 1);
    int dest = write_param(_curr_instr, 2);

    _instructions[dest] = lhs * rhs;
}

void intcode::inp()
{
    int dest = write_param(_curr_instr, 0);
    intcode_std_out("intcode_std_input > ", false);
    std::string input;
    std::getline(std::cin, input);
    _instructions[dest] = std::stoi(input);
}

void intcode::out()
{
    int val = read_param(_curr_instr, 0);
    intcode_std_out("intcode_std_output > ", false);
    std::cout << val << "\n";
}

void intcode::jump_if_true()
{
    int test_val = read_param(_curr_instr, 0);
    int jump = read_param(_curr_instr, 1);
    if (test_val != 0)
        _ip = jump;
}

void intcode::jump_if_false()
{
    int test_val = read_param(_curr_instr, 0);
    int jump = read_param(_curr_instr, 1);
    if (test_val == 0)
        _ip = jump;
}

void intcode::less_than()
{
    int lhs = read_param(_curr_instr, 0);
    int rhs = read_param(_curr_instr, 1);
    int dest = write_param(_curr_instr, 2);
    _instructions[dest] = (lhs < rhs) ? 1 : 0;
}

void intcode::equals()
{
    int lhs = read_param(_curr_instr, 0);
    int rhs = read_param(_curr_instr, 1);
    int dest= write_param(_curr_instr, 2);
    _instructions[dest] = (lhs == rhs) ? 1 : 0;
}

void intcode::terminate()
{
    _ip = INT_MAX;
}

instruction intcode::fetch_instruction()
{
    int instr = fetch_int();
    int opcode = instr % 100;
    int param_mode1 = (instr / 100)   % 10;
    int param_mode2 = (instr / 1000)  % 10;
    int param_mode3 = (instr / 10000) % 10;

    instruction curr_instruction;
    curr_instruction.raw_opcode = instr;
    curr_instruction.opcode = opcode;
    curr_instruction.param_count = _opcode_map[opcode].param_count;

    for (int i = 0; i < curr_instruction.param_count; i++)
    {
        int raw = fetch_int();
        auto mode = (i == 0) ? param_mode1 :
                    (i == 1) ? param_mode2 :
                    (i == 2) ? param_mode3 : 0;

        curr_instruction.params[i].value = raw;
        curr_instruction.params[i].mode = static_cast<memory_mode>(mode);
    }

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

int intcode::read_param(const instruction& instr, int i)
{
    int raw = instr.params[i].value;

    switch (instr.params[i].mode)
    {
        case memory_mode::position:
            return _instructions[raw];
        case memory_mode::immediate:
            return raw;
        default:
            throw std::runtime_error("Unknown param mode for reading");
    }
}

int intcode::write_param(const instruction& instr, int i)
{
    int raw = instr.params[i].value;
    return raw;
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
