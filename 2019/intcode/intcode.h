#ifndef JUMI_INTCODE_H
#define JUMI_INTCODE_H
#include <array>
#include <iosfwd>
#include <string>
#include <vector>
#include <unordered_map>

enum class memory_mode
{
    position  = 0,
    immediate = 1,
};

std::string memory_mode_to_string(memory_mode mode);

struct param
{
    int value;
    memory_mode mode;
};

struct instruction
{
    int raw_opcode;
    int opcode;
    std::array<param, 3> params;
    int param_count;
};

std::ostream& operator<<(std::ostream& os, const instruction& instr);

class intcode
{
    struct opcode_info
    {
        using member_function = void(intcode::*)(void);
        member_function func;
        std::string name;
        int param_count;
    };
public:
    intcode(const std::string& instruction_stream);
    void execute_program();

private:
    std::vector<int> _instructions;
    int _ip;
    instruction _curr_instr;
    std::unordered_map<int, opcode_info> _opcode_map = 
    {
        { 1,  { &intcode::add, "add",                     3 }},
        { 2,  { &intcode::mul, "mul",                     3 }},
        { 3,  { &intcode::inp, "inp",                     1 }},
        { 4,  { &intcode::out, "out",                     1 }},
        { 5,  { &intcode::jump_if_true, "jump_if_true",   2 }},
        { 6,  { &intcode::jump_if_false, "jump_if_false", 2 }},
        { 7,  { &intcode::less_than, "less_than",         3 }},
        { 8,  { &intcode::equals, "equals",               3 }},
        { 99, { &intcode::terminate, "terminate",         0 }},
    };

    void add();
    void mul();
    void inp();
    void out();
    void jump_if_true();
    void jump_if_false();
    void less_than();
    void equals();
    void terminate();

    instruction fetch_instruction();
    int fetch_int();
    int read_param(const instruction& instr, int i);
    int write_param(const instruction& instr, int i);
    void intcode_std_out(const std::string& output, bool newline = true);
    std::vector<int> get_opcodes(std::string instruction_stream);
};

#endif
