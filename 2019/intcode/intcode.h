#ifndef JUMI_INTCODE_H
#define JUMI_INTCODE_H
#include <iosfwd>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

enum class memory_mode
{
    position  = 0,
    immediate = 1,
};

std::string memory_mode_to_string(memory_mode mode);

struct instruction
{
    int raw_opcode;
    int opcode;
    std::vector<memory_mode> parameter_memory_modes;
    std::optional<int> param1;
    std::optional<int> param2;
    std::optional<int> param3;
};

std::ostream& operator<<(std::ostream& os, const instruction& instr);

class intcode
{
    struct opcode_info
    {
        using member_function = void(intcode::*)(void);
        member_function func;
        std::string name;
    };
public:
    intcode(const std::string& instruction_stream);
    void execute_program();

private:
    std::vector<int> _instructions;
    int _ip;
    instruction _current_instruction;
    std::unordered_map<int, opcode_info> _opcode_map = 
    {
        { 1,  { &intcode::add, "add"                     }},
        { 2,  { &intcode::mul, "mul"                     }},
        { 3,  { &intcode::inp, "inp"                     }},
        { 4,  { &intcode::out, "out"                     }},
        { 5,  { &intcode::jump_if_true, "jump_if_true"   }},
        { 6,  { &intcode::jump_if_false, "jump_if_false" }},
        { 7,  { &intcode::less_than, "less_than"         }},
        { 8,  { &intcode::equals, "equals"               }},
        { 99, { &intcode::terminate, "terminate"         }},
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
    void fetch_params(int num_params);
    int decode_memory_mode(const std::optional<int>& value, memory_mode mode);
    void intcode_std_out(const std::string& output, bool newline = true);
    std::vector<int> get_opcodes(std::string instruction_stream);
};

#endif
