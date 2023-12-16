#include "jumi_utils.h"
using jumi::operator<<;

constexpr const char* filepath { "Day8_input.txt" };

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> contents = jumi::vectorize_file(file);

    for (const auto& i : contents)
    {
        std::cout << i << '\n';
    }

    return 0;
}
