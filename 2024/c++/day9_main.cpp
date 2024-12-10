#include "jumi_utils.h"
#include <iostream>
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

    std::string disk_str_representation() const
    {
        std::stringstream ss;

        for (const data_block& block : _data_blocks)
        {
            if (block.ids.empty())
            {
                int count = block.capacity;
                while (count > 0)
                {
                    ss << '.';
                    --count;
                }
            }
            else
            {
                for (const int i : block.ids)
                {
                    std::string num_str = std::to_string(i);
                    for (const char c : num_str)
                    {
                        ss << c;
                    }
                }
            }
        }

        return ss.str();
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
    std::string rep = d.disk_str_representation();
    std::cout << "test_str: " << test_str << '\n';
    std::cout << rep << '\n';
}

