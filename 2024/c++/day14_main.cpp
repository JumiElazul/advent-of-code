#include "jumi_utils.h"
#include <cassert>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

static int room_width = 101;
static int room_height = 103;

struct vec2
{
    int x;
    int y;

    bool operator==(const vec2& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool operator!=(const vec2& rhs) const
    {
        return !(*this == rhs);
    }
};

struct robot
{
    int x_pos;
    int y_pos;
    vec2 direction;
};

std::ostream& operator<<(std::ostream& os, const robot& r)
{
    os << "x_pos: " << r.x_pos << "  y_pos: " << r.y_pos << "  velocity: " << r.direction.x << " " << r.direction.y;
    return os;
}

enum class quadrant
{
    top_left, top_right, bottom_left, bottom_right,
    invalid
};

quadrant determine_quadrant(int x, int y, int width, int height)
{
    int mid_x = width / 2;
    int mid_y = height / 2;

    if (x < mid_x && y < mid_y)
        return quadrant::top_left;
    else if (x > mid_x && y < mid_y)
        return quadrant::top_right;
    else if (x < mid_x && y > mid_y)
        return quadrant::bottom_left;
    else if (x > mid_x && y > mid_y)
        return quadrant::bottom_right;
    else
        return quadrant::invalid;
}

std::vector<robot> get_robots(const std::vector<std::string>& contents)
{
    std::vector<robot> robots;
    std::regex pattern("-?\\d+,-?\\d+");
    std::smatch match;

    for (const std::string& line : contents)
    {
        std::string::const_iterator it = line.begin();
        robot r{ -1, -1, 0, 0 };

        while (it != line.end())
        {
            std::regex_search(it, line.end(), match, pattern);
            std::string match_str = match.str();
            std::vector<std::string> pair = jumi::split(match_str, ',');

            int first = std::stoi(pair[0]);
            int second = std::stoi(pair[1]);

            if (r.x_pos == -1 && r.y_pos == -1)
            {
                r.x_pos = first;
                r.y_pos = second;
            }
            else
            {
                r.direction = vec2{ first, second };
            }

            it = match.suffix().first;
        }

        robots.push_back(r);
    }

    return robots;
}

void part_one(std::vector<robot> robots)
{

    for (int i = 0; i < 100; ++i)
    {
        for (robot& r : robots)
        {
            r.x_pos = (r.x_pos + r.direction.x + room_width) % room_width;
            r.y_pos = (r.y_pos + r.direction.y + room_height) % room_height;
        }
    }

    int top_left = 0;
    int top_right = 0;
    int bottom_left = 0;
    int bottom_right = 0;

    for (const robot& r : robots)
    {
        quadrant q = determine_quadrant(r.x_pos, r.y_pos, room_width, room_height);
        switch (q)
        {
            case quadrant::top_left:
                ++top_left;
                break;
            case quadrant::top_right:
                ++top_right;
                break;
            case quadrant::bottom_left:
                ++bottom_left;
                break;
            case quadrant::bottom_right:
                ++bottom_right;
                break;
            case quadrant::invalid:
                break;
        }
    }

    int result = top_left * top_right * bottom_left * bottom_right;
    std::cout << "[Part One] The safety factor after 100 seconds is " << result << '\n';
}

void part_two(std::vector<robot> robots)
{
    std::cout << "Starting part two simulation, type any key to step, q will exit\n";

    std::vector<std::vector<int>> room(room_height);
    for (int i = 0; i < room_height; ++i)
        room[i].resize(room_width);

    for (const robot& r : robots)
    {
        room[r.y_pos][r.x_pos] += 1;
    }

    int step = 79;
    int increment = 101;
    // int step = 52;
    // int increment = 103;

    for (int i = 0; i < step; ++i)
    {
        // Step 52 first horizontal
        // Step 155 next horizontal
        // Step 79 first vertical
        // Step 180 first horizontal
        for (robot& r : robots)
        {
            room[r.y_pos][r.x_pos] -= 1;
            r.x_pos = (r.x_pos + r.direction.x + room_width) % room_width;
            r.y_pos = (r.y_pos + r.direction.y + room_height) % room_height;
            room[r.y_pos][r.x_pos] += 1;
        }
    }

    std::string input;

    while (input != "q" || input != "quit")
    {
        std::cout << "Step: " << step << '\n';
        for (const auto& line : room)
        {
            for (const int i : line)
            {
                if (i != 0)
                    std::cout << i;
                std::cout << ' ';
            }
            std::cout << '\n';
        }

        std::getline(std::cin, input);

        if (input == "q" || input == "quit")
            break;

        input == "b" ? step -= increment : step += increment;

        for (robot& r : robots)
        {
            for (int i = 0; i < increment; ++i)
            {
                room[r.y_pos][r.x_pos] -= 1;
                if (input == "b")
                {
                    r.x_pos = (r.x_pos - r.direction.x + room_width) % room_width;
                    r.y_pos = (r.y_pos - r.direction.y + room_height) % room_height;
                }
                else
                {
                    r.x_pos = (r.x_pos + r.direction.x + room_width) % room_width;
                    r.y_pos = (r.y_pos + r.direction.y + room_height) % room_height;
                }
                room[r.y_pos][r.x_pos] += 1;
            }
        }
    }
}

int main()
{
    std::fstream file = jumi::open_file("input/day14_input.txt");
    std::vector<std::string> contents = jumi::read_lines(file);
    std::vector<robot> robots = get_robots(contents);

    part_one(robots);
    part_two(robots);
}
