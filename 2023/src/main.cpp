#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

constexpr const char* day1_path = "day1_input.txt";

void check_file(const std::ifstream& file) {
    if (!file) {
        std::cout << "Error opening file\n";
        throw std::exception();
    }
}

std::string string_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    check_file(file);

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::vector<std::string> vectorize_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    check_file(file);

    std::string line;
    std::vector<std::string> vector;

    while (std::getline(file, line)) {
         vector.push_back(line);
    }

    return vector;
}

void day1_part_one() {
    std::vector<std::string> input = vectorize_file(day1_path);
    int sum = 0;

    auto extract_value = [](const std::string& line) -> int {
        auto first = std::find_if(line.begin(), line.end(), [](unsigned char c) {
            return std::isdigit(c);
        });

        auto last = std::find_if(line.rbegin(), line.rend(), [](unsigned char c) {
            return std::isdigit(c);
        });

        if (first == line.end() || last == line.rend()) {
            throw std::runtime_error("No digit found in line: " + line);
        }
        int tens = *first - '0';
        int ones = *last  - '0';
        return tens * 10 + ones;
    };

    for (const std::string& line : input) {
        sum += extract_value(line);
    }

    std::cout << sum << '\n';
}

void day1_part_two() {
    std::vector<std::string> input = vectorize_file(day1_path);
    int sum = 0;


}

int main() {
    day1_part_one();
    day1_part_two();
}
