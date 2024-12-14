#include "jumi_utils.h"
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

constexpr const char* filepath = "input/day9_input.txt";

struct data_block
{
    std::vector<int> ids;
    int capacity;
};

std::vector<data_block> get_data_block_representation(const std::string& diskmap)
{
    std::vector<data_block> blocks(diskmap.size());
    int block_id = 0;

    for (size_t i = 0; i < diskmap.size(); ++i)
    {
        const int block_size = diskmap[i] - '0';
        blocks[i].capacity = block_size;
        if (i % 2 == 0)
        {
            blocks[i].ids = std::vector<int>(block_size, block_id);
            ++block_id;
        }
    }

    return blocks;
}

void part_one(std::vector<data_block> data_blocks)
{
    auto find_empty_block = [](const auto& elem) { return elem.ids.size() != static_cast<size_t>(elem.capacity); };
    auto find_file_block = [](const auto &elem) { return elem.ids.size() > 0; };

    std::vector<data_block>::iterator empty_it = std::find_if(data_blocks.begin(), data_blocks.end(), find_empty_block);
    std::vector<data_block>::reverse_iterator file_it = std::find_if(data_blocks.rbegin(), data_blocks.rend(), find_file_block);

    while (empty_it != data_blocks.end() && file_it != data_blocks.rend() && empty_it < std::prev(file_it.base()))
    {
        while (empty_it->ids.size() != static_cast<size_t>(empty_it->capacity) && file_it->ids.size() > 0)
        {
            auto val = file_it->ids.back();
            file_it->ids.pop_back();
            empty_it->ids.emplace_back(val);
        }

        empty_it = std::find_if(empty_it, data_blocks.end(), find_empty_block);
        file_it = std::find_if(file_it, data_blocks.rend(), find_file_block);
    }

    size_t checksum = 0;
    size_t index = 0;
    for (const auto& block : data_blocks)
    {
        for (const auto &id : block.ids)
        {
            checksum += index * id;
            ++index;
        }
    }

    std::cout << "[Part One] The checksum is " << checksum << '\n';
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::string contents = jumi::stringify_file(file);
    std::vector<data_block> data_blocks = get_data_block_representation(contents);
    part_one(data_blocks);
}

