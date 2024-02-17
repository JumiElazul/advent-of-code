#include "jumi_utils.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <functional>

// --- Day 6: Probably a Fire Hazard ---
// Because your neighbors keep defeating you in the holiday house decorating contest year after year, you've decided 
// to deploy one million lights in a 1000x1000 grid.

// Furthermore, because you've been especially nice this year, Santa has mailed you instructions on how to display 
// the ideal lighting configuration.

// Lights in your grid are numbered from 0 to 999 in each direction; the lights at each corner are at 0,0, 0,999, 999,999, 
// and 999,0. The instructions include whether to turn on, turn off, or toggle various inclusive ranges given as coordinate 
// pairs. Each coordinate pair represents opposite corners of a rectangle, inclusive; a coordinate pair like 0,0 through 2,2 
// therefore refers to 9 lights in a 3x3 square. The lights all start turned off.

// To defeat your neighbors this year, all you have to do is set up your lights by doing the instructions Santa sent you in order.

// For example:

// turn on 0,0 through 999,999 would turn on (or leave on) every light.
// toggle 0,0 through 999,0 would toggle the first line of 1000 lights, turning off the ones that were on, and turning on the ones 
// that were off.
// turn off 499,499 through 500,500 would turn off (or leave off) the middle four lights.
// After following the instructions, how many lights are lit?

// --- Part Two ---
// You just finish implementing your winning light pattern when you realize you mistranslated Santa's message from Ancient Nordic Elvish.

// The light grid you bought actually has individual brightness controls; each light can have a brightness of zero or more. The lights 
// all start at zero.

// The phrase turn on actually means that you should increase the brightness of those lights by 1.
// The phrase turn off actually means that you should decrease the brightness of those lights by 1, to a minimum of zero.
// The phrase toggle actually means that you should increase the brightness of those lights by 2.

// What is the total brightness of all lights combined after following Santa's instructions?

// For example:

// turn on 0,0 through 0,0 would increase the total brightness by 1.
// toggle 0,0 through 999,999 would increase the total brightness by 2000000.

constexpr int row_size = 1000;
constexpr int col_size = 1000;

enum class instruction
{
    turn_on,
    turn_off,
    toggle
};

std::string instruct_to_string(instruction instruct)
{
    switch (instruct)
    {
        case instruction::turn_on:
            return "turn on";
        case instruction::turn_off:
            return "turn off";
        case instruction::toggle:
            return "toggle";
        default:
            return "Unknown";
    }
}

struct instruction_set
{
    size_t start_row;
    size_t start_col;
    size_t end_row;
    size_t end_col;
    instruction instruction_;
};

std::ostream& operator<<(std::ostream& os, const instruction_set& set)
{
    os << "Start row: " << set.start_row 
        << " Start col: " << set.start_col
        << " End row: " << set.end_row
        << " End col: " << set.end_col
        << " Instruction: " << instruct_to_string(set.instruction_);
    return os;
}

instruction_set get_instruction_set(const std::string& line)
{
    instruction_set set;
    std::vector<std::string> split = jumi::split(line, ' ');

    if (split.at(0) == "toggle")
        set.instruction_ = instruction::toggle;
    else if (split.at(1) == "off")
        set.instruction_ = instruction::turn_off;
    else if (split.at(1) == "on")
        set.instruction_ = instruction::turn_on;

    size_t start_index = 1;
    size_t end_index = 3;
    if (set.instruction_ != instruction::toggle)
    {
        ++start_index;
        ++end_index;
    }

    std::vector<std::string> start_split{ jumi::split(split.at(start_index), ',') };
    std::vector<std::string> end_split{ jumi::split(split.at(end_index), ',') };

    set.start_row = std::stoull(start_split.at(0));
    set.end_row = std::stoull(end_split.at(0));
    set.start_col = std::stoull(start_split.at(1));
    set.end_col = std::stoull(end_split.at(1));

    return set;
}

void execute_instruction(std::array<std::array<int, 1000>, 1000>& lights, instruction_set set, const std::function<void(int&, instruction)>& strategy)
{
    size_t start_row = set.start_row;
    size_t end_row = set.end_row;
    size_t start_col = set.start_col;
    size_t end_col = set.end_col;
    instruction instruct = set.instruction_;

    for (size_t i = start_col; i <= end_col; ++i)
    {
        for (size_t j = start_row; j <= end_row; ++j)
        {
            strategy(lights[i][j], instruct);
        }
    }
}

size_t count_lights(const std::array<std::array<int, 1000>, 1000>& lights)
{
    size_t result = 0;
    for (size_t i = 0; i < lights.size(); ++i)
    {
        for (size_t j = 0; j < lights[i].size(); ++j)
        {
            result += static_cast<size_t>(lights[i][j]);
        }
    }

    return result;
}

void part_one()
{
    std::fstream file{ "Day6_input.txt" };
    std::vector<std::string> lines{ jumi::read_lines(file) };

    static std::array<std::array<int, row_size>, col_size> lights{ 0 };

    for (const std::string& line : lines)
    {
        instruction_set set{ get_instruction_set(line) };
        auto strategy_part_one = [](int& light, instruction instruct) {
            switch (instruct)
            {
                case instruction::toggle:
                    light = !light;
                    break;
                case instruction::turn_on:
                    light = true;
                    break;
                case instruction::turn_off:
                    light = false;
                    break;
            }
        };

        execute_instruction(lights, set, strategy_part_one);
    }

    size_t light_count = count_lights(lights);
    std::cout << "The total number of lights on is: " << light_count << '\n';
}

void part_two()
{
    std::fstream file{ "Day6_input.txt" };
    std::vector<std::string> lines{ jumi::read_lines(file) };

    static std::array<std::array<int, row_size>, col_size> lights{ 0 };

    for (const std::string& line : lines)
    {
        instruction_set set{ get_instruction_set(line) };
        auto strategy_part_two = [](int& light, instruction instruct) {
            switch (instruct)
            {
                case instruction::toggle:
                    light += 2;
                    break;
                case instruction::turn_on:
                    light += 1;
                    break;
                case instruction::turn_off:
                    light -= 1;
                    break;
            }
            light = std::max(light, 0);
        };

        execute_instruction(lights, set, strategy_part_two);
    }

    size_t light_count = count_lights(lights);
    std::cout << "The total brightness of lights is: " << light_count << '\n';
}

int main()
{
    part_one();
    part_two();
}
