#include "jumi_utils.h"
#include "intcode.h"
#include <algorithm>
#include <cassert>
#include <limits>
#include <string>
#include <vector>

void day5(int argc, char* argv[])
{
    std::string arg = argv[1];
    std::fstream file = jumi::open_file(arg);
    std::string instruction_stream = jumi::stringify_file(file);

    intcode c(instruction_stream);
    c.execute_program();
}

void day7(int argc, char* argv[], int part)
{
    std::string arg = argv[1];
    std::fstream file = jumi::open_file(arg);
    std::string instruction_stream = jumi::stringify_file(file);

    auto part1 = [](std::string instructions) {
        std::array<int, 5> phase_inputs = { 0, 1, 2, 3, 4 };
        std::vector<int> output;
        int max = std::numeric_limits<int>::min();

        while (true)
        {
            int input = 0;

            for (size_t i = 0; i < phase_inputs.size(); ++i)
            {
                intcode computer(instructions, { phase_inputs[i] });
                computer.add_input(input);
                computer.execute_program();

                output = computer.output();
                assert(output.size() == 1);
                input = output[0];
            }

            if (input > max)
                max = input;

            if (!std::next_permutation(phase_inputs.begin(), phase_inputs.end()))
            {
                break;
            }
        }

        std::cout << "[Part One] The maximum number that can be passed to thrusters is: " << max << '\n';
    };

    auto part2 = [](std::string& instructions) {
        std::array<int, 5> phase_inputs = { 5, 6, 7, 8, 9 };

        std::vector<int> output;
        int max = std::numeric_limits<int>::min();

        while (true)
        {
            int input = 0;

            for (size_t i = 0; i < phase_inputs.size(); ++i)
            {
                intcode computer(instructions, { phase_inputs[i] });
                computer.add_input(input);
                computer.execute_program();

                output = computer.output();
                assert(output.size() == 1);
                input = output[0];
            }

            if (input > max)
                max = input;

            if (!std::next_permutation(phase_inputs.begin(), phase_inputs.end()))
            {
                break;
            }
        }

        std::cout << "[Part Two] The maximum number that can be passed to thrusters is: " << max << '\n';
    };

    if (part == 1)      part1(instruction_stream);
    else if (part == 2) part2(instruction_stream);
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    day7(argc, argv, 2);
}
