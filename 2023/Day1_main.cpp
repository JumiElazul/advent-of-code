#include "jumi_utils.h"
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <map>

constexpr const char* filepath{ "Day1_input.txt" };

std::unordered_map<std::string, std::string> umap
{
    { "one",   "1" },
    { "two",   "2" },
    { "three", "3" },
    { "four",  "4" },
    { "five",  "5" },
    { "six",   "6" },
    { "seven", "7" },
    { "eight", "8" },
    { "nine",  "9" },
};

int GetFirstNum(const std::string& line)
{
    std::string result;
    bool breakLoop = false;;

    // Find first match
    for (int i = 0; i < line.length(); ++i)
    {
        if (breakLoop)
            break;

        // Check for digit
        if (std::isdigit(line[i]))
        {
            result += line[i];
            breakLoop = true;
        }
        // Check for substring
        else
        {
            for (int len = 1; len <= 5 && i + len <= line.length(); ++len) 
            {
                std::string substr = line.substr(i, len);

                if (umap.find(substr) != umap.end()) 
                {
                    result += umap[substr];
                    breakLoop = true;
                }
            }
        }
    }

    return std::stoi(result);
}

int GetLastNum(const std::string& line)
{
    std::string result;
    bool breakLoop = false;

    // Find last match
    for (int i = line.length() - 1; i >= 0; --i)
    {
        if (breakLoop)
            break;

        // Check for digit
        if (std::isdigit(line[i]))
        {
            result += line[i];
            breakLoop = true;
        }
        // Check for substring
        else
        {
            for (int len = 1; len <= 5 && i + len <= line.length(); ++len) 
            {
                std::string substr = line.substr(i, len);

                if (umap.find(substr) != umap.end()) 
                {
                    result += umap[substr];
                    breakLoop = true;
                }
            }
        }
    }

    return std::stoi(result);
}

int main()
{
    std::ifstream file(filepath);

    if (!file)
    {
        std::cout << "File could not be opened\n";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::vector<std::string> lines = jumi::split(ss.str(), '\n');

    int totalSum{};

    for (const std::string& line : lines)
    {
        int first = GetFirstNum(line);
        int last = GetLastNum(line);
        int result = std::stoi(std::to_string(first) + std::to_string(last));

        std::cout << "Result for: " << line << ": [ " << result << " ]\n";

        totalSum += result;
    }

    std::cout << "TotalSum: " << totalSum << '\n';

    return 0;
}
