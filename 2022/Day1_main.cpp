#include "jumi_utils.h"
#include <algorithm>
#include <queue>
#include <vector>

constexpr const char* filepath = "Day1_input.txt";

std::vector<int> get_top_nth_elements(const std::priority_queue<int>& prio_queue, size_t nth)
{
    std::priority_queue<int> temp_heap = prio_queue;
    std::vector<int> result;
    result.reserve(nth);

    for (size_t i = 0; i < nth; ++i)
    {
        result.emplace_back(temp_heap.top());
        temp_heap.pop();
    }

    return result;
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::priority_queue<int, std::vector<int>, std::less<int>> max_heap;
    std::string line;

    int sum = 0;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            max_heap.push(sum);
            sum = 0;
        }
        else
        {
            sum += std::stoi(line);
        }
    }

    int total_sum = 0;
    std::vector<int> top3 = get_top_nth_elements(max_heap, 3);
    for (const auto& num : top3)
    {
        total_sum += num;
    }
    std::cout << "Sum of top 3 elements: " << total_sum << '\n';

    return 0;
}
