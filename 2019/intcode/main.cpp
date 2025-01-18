#include "jumi_utils.h"
#include "intcode.h"
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::string arg = argv[1];
    std::fstream file = jumi::open_file(arg);
    std::string instruction_stream = jumi::stringify_file(file);

    intcode c(instruction_stream);
    c.execute_program();
}
