#include "jumi_utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const char* filepath { "Day5_input.txt" };

int main()
{
    std::fstream file = jumi::open_file(filepath);

    return 0;
}
