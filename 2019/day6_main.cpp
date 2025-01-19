#include "jumi_utils.h"
#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>

using orbit_umap = std::unordered_map<std::string, std::vector<std::string>>;
orbit_umap build_orbit_map(const std::vector<std::string>& lines)
{
    orbit_umap orbit_map;

    for (const std::string& line : lines)
    {
        std::vector<std::string> split = jumi::split(line, ')');
        assert(split.size() == 2);
        orbit_map[split[0]].push_back(split[1]);
    }

    return orbit_map;
}

std::ostream& operator<<(std::ostream& os, const orbit_umap& map)
{
    for (const auto& i : map)
    {
        os << i.first << " : ";
        for (const auto& j : i.second)
            os << j << " ";

        os << '\n';
    }
    return os;
}

void part_one(const orbit_umap& orbit_map)
{
    int orbit_size = 0;
    std::string start = "COM";

    auto it = orbit_map.find(start);

}

int main()
{
    std::fstream file = jumi::open_file("input/day6_input.txt");
    std::vector<std::string> lines = jumi::read_lines(file);
    orbit_umap orbit_map = build_orbit_map(lines);
    part_one(orbit_map);
    std::cout << orbit_map << '\n';
}
