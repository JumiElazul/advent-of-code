#include "jumi_utils.h"
#include <iostream>
#include <vector>
#include <string>

class cpu
{
public:

private:
    int _register_a;
    int _register_b;
    int _register_c;
};

int main()
{
    std::fstream file = jumi::open_file("input/day17_input.txt");
    std::vector<std::string> contents = jumi::read_lines(file);
}
