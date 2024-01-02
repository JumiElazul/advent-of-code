#include "jumi_utils.h"
#include <algorithm>
#include <array>
#include <iterator>
#include <vector>
#include <unordered_map>

constexpr const char* filepath = "Day1_input.txt";

struct Elf
{
    std::int32_t elf_number = 0;
    std::int64_t calories = 0;
};

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> lines = jumi::read_lines(file);
    std::unordered_map<std::int32_t, std::int64_t> elves;

    std::int64_t sum_total = 0;
    std::int32_t elf_number = 1;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        if (lines[i].empty())
        {
            elves[elf_number++] = sum_total;
            sum_total = 0;
            continue;
        }
        sum_total += std::stoll(lines[i]);
    }
    elves[elf_number] = sum_total;

    auto it = std::max_element(elves.begin(), elves.end(),
            [](const auto& a, const auto& b) {
                return a.second < b.second; 
            });

    std::cout << "Max element is: " << it->first << " with value: " << it->second << '\n';

    std::vector<std::int64_t> elements;
    elements.reserve(elves.size());
    for (std::unordered_map<std::int32_t, std::int64_t>::const_iterator it = elves.begin(); it != elves.end(); ++it)
    {
        elements.push_back(it->second);
    }

    std::nth_element(elements.begin(), elements.begin() + 3, elements.end(), std::greater<std::int64_t>());
    std::sort(elements.begin(), elements.begin() + 3, std::greater<std::int64_t>());

    if (elements.size() > 3)
    {
        elements.resize(3);
    }

    std::int64_t sum = 0;
    for (const auto& num : elements)
    {
        sum += num;
    }
    std::cout << "Max 3 elements sum: " << sum << '\n';

    return 0;
}
