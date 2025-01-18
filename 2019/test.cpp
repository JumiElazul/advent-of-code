#include "jumi_utils.h"
#include "intcode/intcode.h"
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::fstream file = jumi::open_file(argv[1]);
    std::string contents = jumi::stringify_file(file);
    std::vector<int> opcodes = get_opcodes(contents);
    intcode c(opcodes);
    c.execute_program();
}
