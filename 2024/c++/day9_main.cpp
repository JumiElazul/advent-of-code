#include "jumi_utils.h"
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

constexpr const char* filepath = "input/day9_input.txt";

struct data_block
{
    int id;
};

std::vector<data_block> get_data_block_representation(const std::string& diskmap)
{
    std::vector<data_block> blocks;
    blocks.reserve(diskmap.size());
    for (int i = 0; i < diskmap.size(); ++i)
    {
        int count = diskmap[i] - '0';
        while (count > 0)
        {
            if (i % 2 == 0)
            {
                blocks.emplace_back(i / 2);
            }
            else
            {
                blocks.emplace_back(-1);
            }
            --count;
        }
    }

    return blocks;
}

size_t compute_checksum(const std::vector<data_block>& data_blocks)
{
    size_t checksum = 0;

    for (int i = 0; i < data_blocks.size(); ++i)
    {
        if (data_blocks[i].id != -1)
        {
            checksum += (i * data_blocks[i].id);
        }
    }

    return checksum;
}

void part_one(std::vector<data_block>& data_blocks)
{
    auto find_empty = [](const data_block& block) -> bool {
        return block.id == -1;
    };

    auto find_file = [](const data_block& block) {
        return block.id >= 0;
    };

    std::vector<data_block>::iterator empty = std::find_if(data_blocks.begin(), data_blocks.end(), find_empty);
    std::vector<data_block>::reverse_iterator file = std::find_if(data_blocks.rbegin(), data_blocks.rend(), find_file);

    while (empty != data_blocks.end() && file != data_blocks.rend() && empty < std::prev(file.base())) 
    {
        *empty = *file;
        file->id = -1;

        empty = std::find_if(empty, data_blocks.end(), find_empty);
        file = std::find_if(file, data_blocks.rend(), find_file);
    }

    size_t checksum = compute_checksum(data_blocks);
    std::cout << "[Part One] The checksum is " << checksum << '\n';
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::string contents = jumi::stringify_file(file);
    std::vector<data_block> data_blocks = get_data_block_representation(contents);
    part_one(data_blocks);
}

