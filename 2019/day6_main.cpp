#include "jumi_utils.h"
#include <cassert>
#include <limits>
#include <string>
#include <vector>
#include <unordered_map>

using orbit_umap = std::unordered_map<std::string, std::string>;
orbit_umap build_orbit_map(const std::vector<std::string>& lines)
{
    orbit_umap orbit_map;

    for (const std::string& line : lines)
    {
        std::vector<std::string> split = jumi::split(line, ')');
        const std::string& parent = split[0];
        const std::string& child = split[1];
        assert(split.size() == 2);
        orbit_map[child] = parent;
    }

    return orbit_map;
}

std::ostream& operator<<(std::ostream& os, const orbit_umap& map)
{
    for (const auto& i : map)
    {
        os << i.first << " : " << i.second << '\n';
    }
    return os;
}

int count_orbits(const orbit_umap& orbit_map, const std::string& node, std::unordered_map<std::string, int>& memo)
{
    auto memoized = memo.find(node);
    if (memoized != memo.end())
        return memo.at(node);

    if (node == "COM")
        return memo["COM"] = 0;

    auto find = orbit_map.find(node);
    if (find == orbit_map.end())
    {
        // Should never hit this case
        return memo[node] = 0;
    }

    const std::string& parent = find->second;
    int result = 1 + count_orbits(orbit_map, parent, memo);
    memo[node] = result;
    return result;
}

void part_one(const orbit_umap& orbit_map)
{
    int orbit_count = 0;
    std::unordered_map<std::string, int> memo;
    memo.reserve(orbit_map.size());

    for (const auto& p : orbit_map)
    {
        const std::string& child = p.first;
        orbit_count += count_orbits(orbit_map, child, memo);
    }

    std::cout << "[Part One] The total number of orbits is: " << orbit_count << '\n';
}

std::vector<std::string> get_ancestors(const orbit_umap& orbit_map, const std::string& start)
{
    std::vector<std::string> ancestors;
    std::string current = start;

    while (true)
    {
        ancestors.push_back(current);

        auto it = orbit_map.find(current);
        if (it == orbit_map.end() || it->second == "COM")
            break;

        current = it->second;
    }

    return ancestors;
}

void part_two(const orbit_umap& orbit_map)
{
    std::unordered_map<std::string, int> visited;

    const std::string you = orbit_map.at("YOU");
    const std::string san = orbit_map.at("SAN");

    std::vector<std::string> you_ancestors = get_ancestors(orbit_map, you);
    std::vector<std::string> san_ancestors = get_ancestors(orbit_map, san);


    std::unordered_map<std::string, int> dist_from_you;
    dist_from_you.reserve(you.size());
    for (int i = 0; i < static_cast<int>(you_ancestors.size()); ++i)
    {
        dist_from_you[you_ancestors[i]] = i;
    }

    int min = std::numeric_limits<int>::max();
    for (int j = 0; j < static_cast<int>(san_ancestors.size()); ++j)
    {
        auto it = dist_from_you.find(san_ancestors[j]);
        if (it != dist_from_you.end())
        {
            min = it->second + j;
            break;
        }
    }

    std::cout << "[Part Two] The minimum orbital transfers is: " << min << '\n';
}

int main()
{
    std::fstream file = jumi::open_file("input/day6_input.txt");
    std::vector<std::string> lines = jumi::read_lines(file);
    orbit_umap orbit_map = build_orbit_map(lines);
    part_one(orbit_map);
    part_two(orbit_map);
}
