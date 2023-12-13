#include "jumi_utils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

constexpr const char* filepath{ "Day2_input.txt" };

struct Game
{
    size_t red{};
    size_t green{};
    size_t blue{};
};

std::ostream& operator<<(std::ostream& os, const Game& game)
{
    os << "[" << game.red << "," << game.green << "," << game.blue << "]";
    return os;
}

int main()
{
    std::ifstream file(filepath);

    if (!file)
    {
        std::cout << "File with path: [" << filepath << "] could not be opened\n";
    }

    std::ostringstream ss;
    ss << file.rdbuf();

    std::vector<std::string> lines = jumi::split(ss.str(), '\n');
    std::vector<Game> games;

    for (std::string& line : lines)
    {
        // Strip game number
        size_t index = line.find(":");
        line = line.substr(index + 2);
        Game game{};

        std::vector<std::string> rounds = jumi::split(line, "; ");

        for (const std::string& round : rounds)
        {
            std::vector<std::string> subrounds = jumi::split(round, ", ");

            for (const std::string& subroundsplit : subrounds)
            {
                std::vector<std::string> finalsplit = jumi::split(subroundsplit, ' ');
                int count = std::stoi(finalsplit[0]);
                std::string color = finalsplit[1];

                std::cout << "COUNT: " << count << "    COLOR: " << color << '\n';

                if (color == "red")
                {
                    std::cout << "Found red\n";
                    if (count > game.red)
                    {
                        std::cout << "Updating red with value " << count << '\n';
                        game.red = count;
                    }
                }
                else if (color == "green")
                {
                    std::cout << "Found green\n";
                    if (count > game.green)
                    {
                        std::cout << "Updating green with value " << count << '\n';
                        game.green = count;
                    }
                }
                else if (color == "blue")
                {
                    std::cout << "Found blue\n";
                    if (count > game.blue)
                    {
                        std::cout << "Updating blue with value " << count << '\n';
                        game.blue = count;
                    }
                }
                std::cout << "------------------------------\n";
            }
        }
        games.push_back(game);
    }

    int sum{};
    for (const Game& game : games)
    {
        int square = game.red * game.blue * game.green;
        sum += square;
    }

    std::cout << "Total sum: " << sum << '\n';

    return 0;
}
