#include "jumi_utils.h"
#include <iostream>
#include <functional>
#include <unordered_map>

// --- Day 3: Perfectly Spherical Houses in a Vacuum ---
// Santa is delivering presents to an infinite two-dimensional grid of houses.

// He begins by delivering a present to the house at his starting location, and then an elf at the North Pole calls him
// via radio and tells him where to move next. Moves are always exactly one house to the north (^), south (v), east (>),
// or west (<). After each move, he delivers another present to the house at his new location.

// However, the elf back at the north pole has had a little too much eggnog, and so his directions are a little off, and 
// Santa ends up visiting some houses more than once. How many houses receive at least one present?

// For example:

// > delivers presents to 2 houses: one at the starting location, and one to the east.
// ^>v< delivers presents to 4 houses in a square, including twice to the house at his starting/ending location.
// ^v^v^v^v^v delivers a bunch of presents to some very lucky children at only 2 houses.

// --- Part Two ---
// The next year, to speed up the process, Santa creates a robot version of himself, Robo-Santa, to deliver presents with him.

// Santa and Robo-Santa start at the same location (delivering two presents to the same starting house), then take turns moving
// based on instructions from the elf, who is eggnoggedly reading from the same script as the previous year.

// This year, how many houses receive at least one present?

// For example:

// ^v delivers presents to 3 houses, because Santa goes north, and then Robo-Santa goes south.
// ^>v< now delivers presents to 3 houses, and Santa and Robo-Santa end up back where they started.
// ^v^v^v^v^v now delivers presents to 11 houses, with Santa going one direction and Robo-Santa going the other.
std::unordered_map<char, std::pair<int, int>> direction_changes
{
    {'<', { -1,  0 }},
    {'>', {  1,  0 }},
    {'^', {  0,  1 }},
    {'v', {  0, -1 }},
};

struct location
{
    int x;
    int y;

    location(int x, int y) 
        : x(x)
        , y(y)
    {}

    bool operator==(const location& other) const { return (x == other.x) && (y == other.y); }
    bool operator!=(const location& other) const { return !(*this == other); }
};

namespace std
{
    template<>
    struct hash<location>
    {
        size_t operator()(const location& loc) const
        {
            return std::hash<int>()(loc.x) 
                ^ (std::hash<int>()(loc.y) << 1);
        }
    };
}

void part_one()
{
    std::fstream file{ jumi::open_file("Day3_input.txt") };
    std::string contents{ jumi::stringify_file(file) };

    std::unordered_map<location, int> houses;
    location curr_location{ 0, 0 };
    houses[curr_location] += 1;

    for (size_t i = 0; i < contents.length(); ++i)
    {
        char dir = contents[i];
        std::pair<int, int> change = direction_changes[dir];
        curr_location.x += change.first;
        curr_location.y += change.second;

        houses[curr_location] += 1;
    }

    std::cout << "The total number of houses visited is: " << houses.size() << '\n';
}

void part_two()
{
    std::fstream file{ jumi::open_file("Day3_input.txt") };
    std::string contents{ jumi::stringify_file(file) };

    std::unordered_map<location, int> houses;
    location santa_curr_location{ 0, 0 };
    location robo_santa_curr_location{ 0, 0 };
    houses[santa_curr_location] += 2;
    bool santa = true;

    for (size_t i = 0; i < contents.length(); ++i)
    {
        char dir = contents[i];
        std::pair<int, int> change = direction_changes[dir];
        if (santa)
        {
            santa_curr_location.x += change.first;
            santa_curr_location.y += change.second;
            houses[santa_curr_location] += 1;
        }
        else
        {
            robo_santa_curr_location.x += change.first;
            robo_santa_curr_location.y += change.second;
            houses[robo_santa_curr_location] += 1;
        }
        santa = !santa;
    }

    std::cout << "The total number of houses visited with Robo-Santa is: " << houses.size() << '\n';
}

int main()
{
    part_one();
    part_two();
}
