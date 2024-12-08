#include "jumi_utils.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

constexpr const char* filepath = "input/day7_input.txt";

struct equation
{
    size_t target;
    std::vector<size_t> operands;
};

using solvable = bool;
static std::vector<std::pair<equation, solvable>> s_part_one_solved;
static std::vector<std::pair<equation, solvable>> s_part_two_solved;

enum class problem_part
{
    one,
    two
};

std::ostream& operator<<(std::ostream& os, const equation& eq)
{
    os << eq.target << ": ";
    for (const size_t i : eq.operands)
    {
        os << i << ' ';
    }
    return os;
}

size_t concat_value(size_t op1, size_t op2)
{
    std::string o1 = std::to_string(op1);
    std::string o2 = std::to_string(op2);
    std::string result = o1 + o2;
    return std::stoull(result);
}

void backtrack(const equation& eq, size_t index, size_t current_val, std::vector<char>& ops,
        std::vector<std::vector<char>>& solutions, problem_part part)
{
    if (index == eq.operands.size() - 1)
    {
        if (current_val == eq.target)
        {
            solutions.push_back(ops);
        }
        return;
    }
    size_t next_operand = eq.operands[index + 1];

    if (part == problem_part::one)
    {
        ops.push_back('+');
        backtrack(eq, index + 1, current_val + next_operand, ops, solutions, problem_part::one);
        ops.pop_back();

        ops.push_back('*');
        backtrack(eq, index + 1, current_val * next_operand, ops, solutions, problem_part::one);
        ops.pop_back();
    }
    else
    {
        ops.push_back('+');
        backtrack(eq, index + 1, current_val + next_operand, ops, solutions, problem_part::two);
        ops.pop_back();

        ops.push_back('*');
        backtrack(eq, index + 1, current_val * next_operand, ops, solutions, problem_part::two);
        ops.pop_back();

        ops.push_back('|');
        size_t concat_val = concat_value(current_val, next_operand);
        backtrack(eq, index + 1, concat_val, ops, solutions, problem_part::two);
        ops.pop_back();
    }
}

bool solve_equation(const equation& eq, problem_part part)
{
    if (eq.operands.empty())
        return false;

    std::vector<char> ops;
    std::vector<std::vector<char>> solutions;

    size_t start_val = eq.operands[0];
    backtrack(eq, 0, start_val, ops, solutions, part);

    if (solutions.empty())
    {
        return false;
    }
    return true;
}

std::vector<equation> get_operations_from_input(const std::vector<std::string>& contents)
{
    std::vector<equation> eqs;
    for (const std::string& line : contents)
    {
        equation new_eq;
        size_t index = line.find(':');
        std::string slice = line.substr(0, index);
        new_eq.target = std::stoull(slice);

        std::string chopped_str = line.substr(index + 2);
        std::vector<std::string> split = jumi::split(chopped_str, ' ');
        for (const std::string& num : split)
        {
            new_eq.operands.push_back(std::stoull(num));
        }
        eqs.push_back(new_eq);
    }
    return eqs;
}

void part_one(const std::vector<equation>& equations)
{
    std::vector<equation> solved;

    for (const auto& eq : equations)
    {
        if (solve_equation(eq, problem_part::one))
        {
            solved.push_back(eq);
            s_part_one_solved.push_back({ eq, true });
        }
        else
        {
            s_part_one_solved.push_back({ eq, false });
        }
    }

    size_t result = 0;
    for (const equation& eq : solved)
    {
        result += eq.target;
    }

    std::cout << "[Part One] The final result is " << result << '\n';
}

void part_two(const std::vector<equation>& equations)
{
    std::vector<equation> solved;

    for (const auto& eq : equations)
    {
        if (solve_equation(eq, problem_part::two))
        {
            solved.push_back(eq);
            s_part_two_solved.push_back({ eq, true });
        }
        else
        {
            s_part_two_solved.push_back({ eq, false });
        }
    }

    size_t result = 0;
    for (const equation& eq : solved)
    {
        result += eq.target;
    }

    std::cout << "[Part Two] The final result is " << result << '\n';
}

int main()
{
    std::fstream file(filepath);
    std::vector<std::string> contents = jumi::read_lines(file);
    std::vector<equation> equations = get_operations_from_input(contents);
    part_one(equations);
    part_two(equations);
    // 169122112716571
}
