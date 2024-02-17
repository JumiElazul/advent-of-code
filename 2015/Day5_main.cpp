#include "jumi_utils.h"
#include <iostream>
#include <string_view>
#include <unordered_map>

// --- Day 5: Doesn't He Have Intern-Elves For This? ---
// Santa needs help figuring out which strings in his text file are naughty or nice.

// A nice string is one with all of the following properties:

// It contains at least three vowels (aeiou only), like aei, xazegov, or aeiouaeiouaeiou.
// It contains at least one letter that appears twice in a row, like xx, abcdde (dd), or aabbccdd (aa, bb, cc, or dd).
// It does not contain the strings ab, cd, pq, or xy, even if they are part of one of the other requirements.
// For example:

// ugknbfddgicrmopn is nice because it has at least three vowels (u...i...o...), a double letter (...dd...), and none of 
// the disallowed substrings.
// aaa is nice because it has at least three vowels and a double letter, even though the letters used by different rules overlap.
// jchzalrnumimnmhp is naughty because it has no double letter.
// haegwjzuvuyypxyu is naughty because it contains the string xy.
// dvszwmarrgswjxmb is naughty because it contains only one vowel.
// How many strings are nice?

// --- Part Two ---
// Realizing the error of his ways, Santa has switched to a better model of determining whether a string is naughty or nice. None of the old 
// rules apply, as they are all clearly ridiculous.

// Now, a nice string is one with all of the following properties:

// It contains a pair of any two letters that appears at least twice in the string without overlapping, like xyxy (xy) or aabcdefgaa (aa),
// but not like aaa (aa, but it overlaps).
// It contains at least one letter which repeats with exactly one letter between them, like xyx, abcdefeghi (efe), or even aaa.
// For example:

// qjhvhtzxzqqjkmpb is nice because is has a pair that appears twice (qj) and a letter that repeats with exactly one letter 
// between them (zxz).
// xxyxx is nice because it has a pair that appears twice and a letter that repeats with one between, even though the letters 
// used by each rule overlap.
// uurcxstgmygtbstg is naughty because it has a pair (tg) but no repeat with a single letter between them.
// ieodomkazucvgmuy is naughty because it has a repeating letter with one between (odo), but no pair that appears twice.
// How many strings are nice under these new rules?

constexpr char vowels[5] { 'a', 'e', 'i', 'o', 'u', };
#define INVALID_STRINGS_LENGTH 4
constexpr const char* invalid_strings[INVALID_STRINGS_LENGTH] { "ab", "cd", "pq", "xy" };

void part_one()
{
    std::fstream file{ jumi::open_file("Day5_input.txt") };
    std::vector<std::string> contents{ jumi::read_lines(file) };
    int nice_count = 0;

    auto check_for_invalidator_strings = [](std::string_view line) -> bool {
        for (int i = 0; i < INVALID_STRINGS_LENGTH; ++i)
        {
            if (line.find(invalid_strings[i]) != std::string::npos)
            {
                return false;
            }
        }
        return true;
    };

    auto check_for_double_chars = [](std::string_view line) {
        for (size_t i = 0; i < line.length() - 1; ++i)
        {
            if (line[i] == line[i + 1])
            {
                return true;
            }
        }

        return false;
    };

    auto check_for_vowel_count = [](std::string_view line) {
        int vowel_count = 0;
        for (const char& c : line)
        {
            auto it = std::find(std::begin(vowels), std::end(vowels), c);

            if (it != std::end(vowels))
            {
                ++vowel_count;
                if (vowel_count >= 3)
                {
                    return true;
                }
            }
        }

        return false;
    };

    for (const std::string& line : contents)
    {
        bool no_invalidators = check_for_invalidator_strings(line);
        bool has_vowels = check_for_vowel_count(line);
        bool double_chars = check_for_double_chars(line);

        if (no_invalidators && has_vowels && double_chars)
        {
            std::cout << "The line [" << line << "] has no invalidators and has at least 3 vowels and 2 repeating chars [NICE]\n";
            ++nice_count;
        }
        else
        {
            std::cout << "The line [" << line << "] either HAS invalidators, or has less than 3 vowels or doesn't contain 2 repeating chars [NAUGHTY]\n";
        }
    }

    std::cout << "The total number of nice strings is [" << nice_count << "]\n";
}

void part_two()
{
    std::fstream file{ jumi::open_file("Day5_input.txt") };
    std::vector<std::string> contents{ jumi::read_lines(file) };
    int nice_count = 0;

    auto gap_repeating = [](std::string_view line) -> bool {
        for (size_t i = 1; i < line.length() - 1; ++i)
        {
            if (line[i - 1] == line[i + 1])
            {
                return true;
            }
        }

        return false;
    };

    auto double_repeating = [](std::string_view line) -> bool {
        for (size_t i = 0; i < line.length() - 1; ++i)
        {
            std::string_view substr = line.substr(i, 2);
            size_t index = line.find(substr, i + 2);

            if (index != std::string::npos)
            {
                return true;
            }
        }
        return false;
    };

    for (std::string_view line : contents)
    {
        bool double_rep = double_repeating(line);
        bool gap_rep = gap_repeating(line);

        if (double_rep && gap_rep)
        {
            ++nice_count;
        }
    }

    std::cout << "Total nice count in part two: " << nice_count << '\n';
}

int main()
{
    part_one();
    part_two();
}
