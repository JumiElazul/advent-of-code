#include "jumi_utils.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>

constexpr const char* filepath = "input/day9_input.txt";

enum class layout
{
    file,
    freespace
};

struct data_block
{
    std::vector<int> ids;
    int capacity;
};

class disk
{
public:
    disk(const std::string& input) : _data_blocks()
    {
        int block_id = 0;
        int i = 0;
        _data_blocks.resize(input.size());

        for (const char& c : input)
        {
            const int block_size = c - '0';
            _data_blocks[i].capacity = block_size;

            if (i % 2 == 0)
            {
                _data_blocks[i].ids = std::vector<int>(block_size, block_id);
                ++block_id;
            }
            ++i;
        }
    }

    size_t compute_checksum()
    {

    }

private:
    std::vector<data_block> _data_blocks;
};

void part_one(std::string& diskmap_representation)
{
    std::cout << diskmap_representation << '\n';
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::string contents = jumi::stringify_file(file);
    std::string test_str = "35493538261755937276396055768953609918499";
    disk d(test_str);
    size_t result = d.compute_checksum();
    std::cout << "test_str: " << test_str << '\n';
    std::cout << "[Part One] The final result checksum is " << result << '\n';
}

