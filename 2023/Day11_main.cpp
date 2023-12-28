#include "jumi_utils.h"

constexpr const char* filepath { "Day11_input.txt" };

int main()
{
    std::fstream input_file = jumi::open_file(filepath);
    std::vector<std::string> lines = jumi::read_lines(input_file);

    for (const auto& line : lines)
    {
        std::cout << line << '\n';
    }
}
