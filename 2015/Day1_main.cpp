#include "jumi_utils.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using jumi::operator<<;

// --- Day 1: Not Quite Lisp ---
// Santa was hoping for a white Christmas, but his weather machine's "snow" function is powered by stars, and he's fresh out!
// To save Christmas, he needs you to collect fifty stars by December 25th. 
// Collect stars by helping Santa solve puzzles. Two puzzles will be made available on each day in the Advent calendar;
// the second puzzle is unlocked when you complete the first. Each puzzle grants one star. Good luck! 

// Here's an easy puzzle to warm you up. 
// Santa is trying to deliver presents in a large apartment building, but he can't find the right floor - the directions
// he got are a little confusing. He starts on the ground floor (floor 0) and then follows the instructions one character at a time. 

// An opening parenthesis, (, means he should go up one floor, and a closing parenthesis, ), means he should go down one floor. 

// The apartment building is very tall, and the basement is very deep; he will never find the top or bottom floors. 

// For example: 

// (()) and ()() both result in floor 0. 
// ((( and (()(()( both result in floor 3. 
// ))((((( also results in floor 3. 
// ()) and ))( both result in floor -1 (the first basement level). 
// ))) and )())()) both result in floor -3. 
// To what floor do the instructions take Santa? 

// --- Part Two ---
// Now, given the same instructions, find the position of the first character that causes him to enter the basement
// (floor -1). The first character in the instructions has position 1, the second character has position 2, and so on.

// For example:

// ) causes him to enter the basement at character position 1.
// ()()) causes him to enter the basement at character position 5.
// What is the position of the character that causes Santa to first enter the basement?

void part_one()
{
    std::fstream file{ jumi::open_file("Day1_input.txt") };
    std::string contents = jumi::stringify_file(file);

    int counter = 0;
    std::for_each(contents.begin(), contents.end(), [&counter](char c) {
        if (c == '(') counter += 1;
        if (c == ')') counter -= 1;
    });

    std::cout << "Santa is on floor: " << counter << "\n";
}

void part_two()
{
    std::fstream file{ jumi::open_file("Day1_input.txt") };
    std::string contents = jumi::stringify_file(file);

    int counter = 0;
    for (size_t i = 0; i < contents.length(); ++i)
    {
        if (contents[i] == '(') counter += 1;
        if (contents[i] == ')') counter -= 1;

        if (counter < 0)
        {
            std::cout << "Santa entered the basement at index: " << i + 1 << "\n";
            break;
        }
    }
}

int main()
{
    part_one();
    part_two();
}
