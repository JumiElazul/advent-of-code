#include "jumi_utils.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <regex>

struct button
{
    long long x_offset;
    long long y_offset;
};

struct prize_location
{
    long long x_location;
    long long y_location;
};

struct game_info
{
    button a;
    button b;
    prize_location location;
};

std::ostream& operator<<(std::ostream& os, const game_info& game)
{
    os << "game: (A Button: " << game.a.x_offset << ", " << game.a.y_offset << ") (B Button: " << game.b.x_offset << ", " <<
        game.b.y_offset <<  "), (Prize Location: " << game.location.x_location << ", " << game.location.y_location << ")";
    return os;
}

std::tuple<bool, long long, long long> check_linear_combination(const long long ax, const long long ay, const long long bx, const long long by, const long long cx, const long long cy)
{
    if ((ax * by) == (bx * ay)) exit(0);

    const long long n1 = (cx * by - cy * bx) / (ax * by - ay * bx);
    const long long n2 = (cx * ay - cy * ax) / (bx * ay - by * ax);

    if (n1 * ax + n2 * bx != cx) return { false, -1, -1 };
    if (n1 * ay + n2 * by != cy) return { false, -1, -1 };
    if (n1 < 0 || n2 < 0) return { false, -1, -1 };

    return { true, n1, n2 };
}

std::vector<game_info> get_game_info(const std::string& contents)
{
    std::vector<game_info> games;
    std::regex game_pattern("Button A: X\\+(\\d+), Y\\+(\\d+)\\nButton B: X\\+(\\d+), Y\\+(\\d+)\\nPrize: X=(\\d+), Y=(\\d+)");
    std::smatch match;
    std::string::const_iterator it = contents.cbegin();

    while (it != contents.cend())
    {
        std::regex_search(it, contents.end(), match, game_pattern);

        if (match.size() == 0)
            break;

        assert(match.size() == 7);
        game_info new_game;

        new_game.a.x_offset = std::stoull(match[1].str());
        new_game.a.y_offset = std::stoull(match[2].str());

        new_game.b.x_offset = std::stoull(match[3].str());
        new_game.b.y_offset = std::stoull(match[4].str());

        new_game.location.x_location = std::stoull(match[5].str());
        new_game.location.y_location = std::stoull(match[6].str());

        games.push_back(new_game);
        it = match.suffix().first;
    }

    return games;
}

void part_one(const std::vector<game_info>& games)
{
    long long total_cost = 0;
    for (const game_info& game : games)
    {
        const auto [possible, n1, n2] = check_linear_combination(
                game.a.x_offset, game.a.y_offset,
                game.b.x_offset, game.b.y_offset,
                game.location.x_location, game.location.y_location);

        if (possible)
        {
            total_cost += (n1 * 3 + n2);
        }
    }

    std::cout << "[Part One] The cheapest way to win the most prizes is " << total_cost << '\n';
}

void part_two(const std::vector<game_info>& games)
{
    long long total_cost = 0;
    long long offset = 10000000000000;
    for (const game_info& game : games)
    {
        const auto [possible, n1, n2] = check_linear_combination(
                game.a.x_offset, game.a.y_offset,
                game.b.x_offset, game.b.y_offset,
                game.location.x_location + offset, game.location.y_location + offset);

        if (possible)
        {
            total_cost += (n1 * 3 + n2);
        }
    }

    std::cout << "[Part Two] The cheapest way to win the most prizes is " << total_cost << '\n';
}

int main()
{
    std::fstream file = jumi::open_file("input/day13_input.txt");
    std::string contents = jumi::stringify_file(file);
    std::vector<game_info> games = get_game_info(contents);
    part_one(games);
    part_two(games);
}
