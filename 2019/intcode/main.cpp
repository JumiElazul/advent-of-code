#include "jumi_utils.h"
#include "intcode.h"
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::string arg = argv[1];
    std::fstream file = jumi::open_file(arg);
    std::string contents = jumi::stringify_file(file);
    std::vector<int> instruction_stream = get_opcodes(contents);
    intcode c(instruction_stream);
    c.execute_program();
}
