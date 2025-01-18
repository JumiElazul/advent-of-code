#ifndef JUMI_INTCODE_H
#define JUMI_INTCODE_H
#include <iosfwd>
#include <string>
#include <vector>
#include <unordered_map>

std::vector<int> get_opcodes(std::string& contents);

struct instruction
{
    int opcode;
    int lhs_loc;
    int rhs_loc;
    int dest;
};

std::ostream& operator<<(std::ostream& os, const instruction& instr);

class intcode
{
public:
    intcode(const std::vector<int>& instruction_stream);
    void execute_program();

    void add();
    void mul();
    void terminate();

private:
    std::vector<int> _instructions;
    int _ip;
    instruction _current_instruction;
    std::unordered_map<int, void(intcode::*)(void)> _opcode_map = 
    {
        { 1, &intcode::add },
        { 2, &intcode::mul },
        { 99, &intcode::terminate },
    };

    instruction fetch_instruction();
};

#endif
