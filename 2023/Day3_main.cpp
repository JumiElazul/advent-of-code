#include "jumi_utils.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

constexpr const char* filepath = "Day3_input.txt";

// ------------------------------
// Part 1
// ------------------------------
std::vector<std::string> vectorize_input(std::fstream& file)
{
    std::vector<std::string> vector;
    std::string line_str;

    while (std::getline(file, line_str))
    {
        vector.push_back(line_str);
    }

    return vector;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vec)
{
    for (const std::string& str : vec)
    {
        os << str << '\n';
    }
    return os;
}

std::string gather_number(const std::string& line, size_t start_index)
{
    std::string number;

    if (!std::isdigit(line[start_index]))
        std::cerr << "line has no digit at start_index: " << start_index << '\n';

    while (start_index < line.length() && std::isdigit(line[start_index]))
    {
        number += line[start_index];
        ++start_index;
    }

    return number;
}

bool is_valid(const std::vector<std::string>& matrix, size_t line_index, size_t start_index, size_t end_index)
{
    auto is_digit_or_dot = [](char ch) {
        return std::isdigit(ch) || ch == '.';
    };

    auto is_in_bounds = [](int index, int length) {
        return index >= 0 && index < length;
    };

    std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };

    for (size_t i = start_index; i <= end_index; ++i)
    {
        for (const auto& dir : directions)
        {
            int new_line = static_cast<int>(line_index) + dir.first;
            int new_index = static_cast<int>(i) + dir.second;

            if (new_line == static_cast<int>(line_index) && (new_index >= static_cast<int>(start_index) && new_index <= static_cast<int>(end_index))) {
                continue;
            }

            if (is_in_bounds(new_line, matrix.size()) && is_in_bounds(new_index, matrix[new_line].size()))
            {
                if (!is_digit_or_dot(matrix[new_line][new_index]))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

int process_matrix(const std::vector<std::string>& matrix)
{
    int finalSum{};
    for (size_t line_index = 0; line_index < matrix.size(); ++line_index)
    {
        size_t start_index = 0;
        while ((start_index = matrix[line_index].find_first_of("123456789", start_index)) != std::string::npos)
        {
            std::string number = gather_number(matrix[line_index], start_index);

            size_t end_index = start_index + number.length() - 1;
            bool valid = is_valid(matrix, line_index, start_index, end_index);

            if (valid)
            {
                finalSum += std::stoi(number);
            }

            start_index = start_index + number.length();
        }
    }
    return finalSum;
}

// ------------------------------
// Part 2
// ------------------------------

std::vector<std::pair<int, int>> get_gear_locations(const std::vector<std::string>& matrix)
{
    std::vector<std::pair<int, int>> locations;
    for (int row = 0; row < matrix.size(); ++row)
    {
        size_t start_index = 0;
        size_t end_index = 0;
        while ((end_index = matrix[row].find_first_of('*', start_index)) != std::string::npos)
        {
            std::pair<int, int> location;
            location.first = row;
            location.second = end_index;
            locations.push_back(location);
            start_index = end_index + 1;
        }
    }
    return locations;
}

struct gear_number
{
    std::string number;
    int row_index = 0;
    int start_col_index = 0;
    int end_col_index = 0;

    bool operator<(const gear_number& other) const 
    {
        if (number != other.number) 
        {
            return number < other.number;
        }
        if (row_index != other.row_index) 
        {
            return row_index < other.row_index;
        }
        if (start_col_index != other.start_col_index) 
        {
            return start_col_index < other.start_col_index;
        }
        return end_col_index < other.end_col_index;
    }
};

std::ostream& operator<<(std::ostream& os, const gear_number& number)
{
    os << "[" << number.number << "," << number.start_col_index << "," << number.end_col_index << "]";
    return os;
}

gear_number get_number(const std::string& line, int row_index, int col_index)
{
    gear_number number;

    int start_col_index = col_index;
    int end_col_index = col_index;

    while (start_col_index > 0 && std::isdigit(line[start_col_index - 1]))
    {
        --start_col_index;
    }
    while (end_col_index < line.size() - 1 && std::isdigit(line[end_col_index + 1]))
    {
        ++end_col_index;
    }
    number.row_index = row_index;
    number.start_col_index = start_col_index;
    number.end_col_index = end_col_index;
    number.number = line.substr(start_col_index, end_col_index - start_col_index + 1);
    return number;
}

bool is_valid_gear_location(const std::vector<std::string>& matrix, const std::pair<int, int>& location)
{
    std::set<gear_number> numbers;

    std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };

    for (const std::pair<int, int>& dir : directions)
    {
        int new_row = location.first + dir.first;
        int new_col = location.second + dir.second;

        if (new_row >= 0 && new_row < matrix.size() && new_col >= 0 && new_col < matrix[location.second].size())
        {
            if (std::isdigit(matrix[new_row][new_col]))
            {
                gear_number number = get_number(matrix[new_row], new_row, new_col);
                numbers.insert(number);
            }
        }
    }

    return numbers.size() == 2;
}

std::vector<std::pair<int, int>> strip_invalid_locations(const std::vector<std::string>& matrix, std::vector<std::pair<int, int>>& gear_locations)
{
    auto new_end = std::remove_if(gear_locations.begin(), gear_locations.end(), 
        [&](const std::pair<int, int>& location) {
            return !is_valid_gear_location(matrix, location);
        });

    gear_locations.erase(new_end, gear_locations.end());
    return gear_locations;
}

int _process_matrix(const std::vector<std::string>& matrix, const std::vector<std::pair<int, int>>& valid_locations)
{
    int sum{};

    std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };

    for (const std::pair<int, int>& location : valid_locations)
    {
        std::set<gear_number> numbers;
        for (const std::pair<int, int>& dir : directions)
        {
            int new_row = location.first + dir.first;
            int new_col = location.second + dir.second;

            if (new_row >= 0 && new_row < matrix.size() && new_col >= 0 && new_col < matrix[location.second].size())
            {
                if (std::isdigit(matrix[new_row][new_col]))
                {
                    gear_number number = get_number(matrix[new_row], new_row, new_col);
                    numbers.insert(number);
                }
            }
        }

        assert(numbers.size() == 2);
        int gear_ratio{ 1 };
        for (const auto& number : numbers)
        {
            gear_ratio *= std::stoi(number.number);
        }
        sum += gear_ratio;
    }

    return sum;
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> matrix = vectorize_input(file);

    // Part 1
    int totalSum = process_matrix(matrix);
    std::cout << "Part1 TotalSum: " << totalSum << '\n';

    // Part 2
    std::vector<std::pair<int, int>> gear_locations = get_gear_locations(matrix);
    gear_locations = strip_invalid_locations(matrix, gear_locations);
    int result = _process_matrix(matrix, gear_locations);

    std::cout << "Part2 GearRatio Result: " << result << '\n';

    return 0;
}
