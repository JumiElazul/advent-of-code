#include "jumi_utils.h"
#include <csignal>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

struct cpu_register
{
    long long a;
    long long b;
    long long c;
};

class cpu
{
using opcode = long long;
using operand = long long;
public:
    cpu(const cpu_register& registers, const std::vector<long long>& instructions)
        : _registers(registers)
        , _instructions(instructions)
        , _ip(0)
        , _halt(false)
        , _output()
    {

    }

    [[nodiscard]] bool halted() const noexcept { return _halt; }
    [[nodiscard]] const std::vector<long long>& get_output() const noexcept { return _output; }

    void run()
    {
        while (!halted())
        {
            execute_instruction();
        }
    }

    void execute_instruction()
    {
        if (_ip >= _instructions.size())
        {
            _halt = true;
            return;
        }

        bool dont_jump = false;
        opcode opcode_ = _instructions[_ip];
        operand operand_ = _instructions[_ip + 1];
        operand combo_operand_ = get_combo_operand(operand_);

        // Combo operands 0 through 3 represent literal values 0 through 3.
        // Combo operand 4 represents the value of register A.
        // Combo operand 5 represents the value of register B.
        // Combo operand 6 represents the value of register C.
        // Combo operand 7 is reserved and will not appear in valid programs.
        switch (opcode_)
        {
            // The adv instruction (opcode 0) performs division. The numerator is the value in the A register. The denominator
            // is found by raising 2 to the power of the instruction's combo operand. (So, an operand of 2 would divide A by 4
            // (2^2); an operand of 5 would divide A by 2^B.) The result of the division operation is truncated to an long longeger 
            // and then written to the A register.
            case 0:
            {
                _registers.a = _registers.a >> combo_operand_;
            } break;
            // The bxl instruction (opcode 1) calculates the bitwise XOR of register B and the instruction's literal operand,
            // then stores the result in register B.
            case 1:
            {
                _registers.b = _registers.b ^ operand_;
            } break;
            // The bst instruction (opcode 2) calculates the value of its combo operand modulo 8 (thereby keeping only its lowest
            // 3 bits), then writes that value to the B register.
            case 2:
            {
                _registers.b = combo_operand_ % 8;
            } break;
            // The jnz instruction (opcode 3) does nothing if the A register is 0. However, if the A register is not zero, it jumps
            // by setting the instruction polong longer to the value of its literal operand; if this instruction jumps, the instruction
            // polong longer is not increased by 2 after this instruction.
            case 3:
            {
                if (_registers.a != 0)
                {
                    _ip = operand_;
                    dont_jump = true;
                }
            } break;
            // The bxc instruction (opcode 4) calculates the bitwise XOR of register B and register C, then stores the result
            // in register B. (For legacy reasons, this instruction reads an operand but ignores it.)
            case 4:
            {
                _registers.b = _registers.b ^ _registers.c;
            } break;
            // The out instruction (opcode 5) calculates the value of its combo operand modulo 8, then outputs that value. (If
            // a program outputs multiple values, they are separated by commas.)
            case 5:
            {
                _output.push_back(combo_operand_ % 8);
            } break;
            // The bdv instruction (opcode 6) works exactly like the adv instruction except that the result is stored in the B
            // register. (The numerator is still read from the A register.)
            case 6:
            {
                _registers.b = _registers.a >> combo_operand_;
            } break;
            // The cdv instruction (opcode 7) works exactly like the adv instruction except that the result is stored in the C
            // register. (The numerator is still read from the A register.)
            case 7:
            {
                _registers.c = _registers.a >> combo_operand_;
            } break;
        }

        if (dont_jump)
            return;

        increment_ip(2);
    }

private:
    cpu_register _registers;
    std::vector<long long> _instructions;
    long long _ip;
    bool _halt;
    std::vector<long long> _output;

    void increment_ip(long long value)
    {
        _ip += value;
    }

    long long get_combo_operand(operand op)
    {
        if (op <= 3)
            return op;

        if (op == 4)
            return _registers.a;
        else if (op == 5)
            return _registers.b;
        else if (op == 6)
            return _registers.c;
        else
            throw std::runtime_error("Invalid operand");
    }
};

cpu_register get_init_registers(const std::vector<std::string>& contents)
{
    std::string target_a = "Register A: ";
    std::string target_b = "Register B: ";
    std::string target_c = "Register C: ";
    std::string cut_str = ": ";

    cpu_register initial{};

    for (const std::string& line : contents)
    {
        if (line.find(target_a) != std::string::npos)
        {
            std::string cut = line.substr(line.find(cut_str) + 2);
            initial.a = std::stoi(cut);
        }
        else if (line.find(target_b) != std::string::npos)
        {
            std::string cut = line.substr(line.find(cut_str) + 2);
            initial.b = std::stoi(cut);
        }
        else if (line.find(target_c) != std::string::npos)
        {
            std::string cut = line.substr(line.find(cut_str) + 2);
            initial.c = std::stoi(cut);
        }
    }

    return initial;
}

std::vector<long long> get_instructions(const std::vector<std::string>& contents)
{
    std::vector<long long> instructions;
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
                instructions.push_back(std::stoll(token));
            }
        }
    }

    return instructions;
}

void part_one(cpu& cpu_)
{
    cpu_.run();
    const std::vector<long long>& output = cpu_.get_output();
    size_t size = output.size();
    for (size_t i = 0; i < size; ++i)
    {
        std::cout << output[i];
        if (i != size - 1)
            std::cout << ',';
    }

    std::cout << '\n';
}

void part_two(const std::vector<std::string>& contents, const std::vector<long long>& instructions)
{
    // cpu_register initial_registers = get_init_registers(contents);
    // initial_registers.a = 0;

    // for (long long i = 0; i < 8; ++i)
    // {
    //     cpu cpu_p2(initial_registers, instructions);
    //     cpu_p2.run();

    //     const std::vector<long long>& output = cpu_p2.get_output();
    //     if (output == instructions)
    //     {
    //         std::cout << "[Part Two] " << i << '\n';
    //     }
    // }
}

int main()
{
    std::fstream file = jumi::open_file("input/day17_input.txt");
    std::vector<std::string> contents = jumi::read_lines(file);
    std::vector<long long> instructions = get_instructions(contents);

    // for (const auto& i : instructions)
    // {
    //     std::cout << i << ',';
    // }

    cpu_register initial_registers = get_init_registers(contents);
    cpu cpu_p1(initial_registers, instructions);
    part_one(cpu_p1);
    part_two(contents, instructions);
}
