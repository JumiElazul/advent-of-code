#include "jumi_utils.h"
#include <iostream>
#include <vector>
#include <string>

std::vector<size_t> get_stones(const std::string& line)
{
    std::vector<size_t> stones;
    std::vector<std::string> split = jumi::split(line, ' ');
    for (const std::string& str : split)
    {
        stones.push_back(std::stoull(str));
    }
    return stones;
}

int count_digits(size_t num)
{
    int count = 0;
    do
    {
        num /= 10;
        ++count;
    } while (num != 0);
    return count;
}

void calculate_part(std::vector<size_t>& stones, int n)
{
    for (int i = 0; i < n; ++i)
    {
        std::vector<size_t> next_stones;
        next_stones.reserve(stones.size() * 2);

        for (auto& stone : stones)
        {
            if (stone == 0)
            {
                next_stones.push_back(1);
            }
            else
            {
                int num_digits = count_digits(stone);
                if (num_digits % 2 == 0)
                {
                    int half = num_digits / 2;
                    size_t divisor = 1;

                    for (int i = 0; i < half; ++i)
                        divisor *= 10;

                    size_t left = stone / divisor;
                    size_t right = stone % divisor;
                    next_stones.push_back(left);
                    next_stones.push_back(right);
                }
                else
                {
                    stone *= 2024;
                    next_stones.push_back(stone);
                }
            }
        }
        stones.swap(next_stones);
    }

    std::cout << "[" << n << " iterations] The final number of stones is " << stones.size() << '\n';
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& list)
{
    for (const auto& elem : list)
        os << elem << '\n';

    return os;
}

int main()
{
    std::fstream file = jumi::open_file("input/day11_input.txt");
    std::string line = jumi::stringify_file(file);
    std::vector<size_t> stones = get_stones(line);
    calculate_part(stones, 25);
    calculate_part(stones, 75);
}
