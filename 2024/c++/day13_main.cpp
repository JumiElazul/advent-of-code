#include "jumi_utils.h"
#include <iostream>
#include <vector>
#include <string>

static int a_button_cost = 3;
static int b_button_cost = 3;

struct button
{
    int x_offset;
    int y_offset;
};

struct prize_location
{
    int x_location;
    int y_location;
};

struct game_info
{
    button a;
    button b;
    prize_location location;
};

std::vector<game_info> get_game_info(const std::vector<std::string>& contents)
{
    std::vector<game_info> games;

    for (int i = 0; i < contents.size(); i += 4)
    {
        game_info curr_game;
        std::string button_target = "X+";
        std::string line = contents[i].substr(contents[i].find_first_of(button_target));

    }

    return games;
}

void part_one(const std::vector<game_info>& games)
{

}

int main()
{
    std::fstream file = jumi::open_file("input/day13_input.txt");
    std::vector<std::string> contents = jumi::read_lines(file);
    std::vector<game_info> games = get_game_info(contents);
    part_one(games);
}
