#include "jumi_utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>

const char* filepath { "Day5_input.txt" };
constexpr const char* seed_to_soil_delim = "seed-to-soil map";
constexpr const char* soil_to_fertilizer_delim = "soil-to-fertilizer map";
constexpr const char* fertilizer_to_water_delim = "fertilizer-to-water map";
constexpr const char* water_to_light_delim = "water-to-light map";
constexpr const char* light_to_temperature_delim = "light-to-temperature map";
constexpr const char* temperature_to_humidity_delim = "temperature-to-humidity map";
constexpr const char* humidity_to_location_delim = "humidity-to-location map";

using jumi::operator<<;

struct destination_source_map
{
    int64 destination_range_start = 0;
    int64 source_range_start      = 0;
    int64 range                   = 0;
};

struct seed_pair
{
    int64 seed_start = 0;
    int64 seed_range = 0;
};

std::ostream& operator<<(std::ostream& os, const destination_source_map& dsm)
{
    os << dsm.destination_range_start << "," << dsm.source_range_start << "," << dsm.range;
    return os;
}

std::ostream& operator<<(std::ostream& os, const seed_pair& pair)
{
    os << '(' << pair.seed_start << "," << pair.seed_range << ')';
    return os;
}

std::vector<int64> get_seeds(const std::string& line)
{
    std::vector<int64> seeds;

    size_t index = line.find_first_of("0123456789");
    std::string new_line = line.substr(index);

    size_t start_pos = 0;
    size_t end_pos = 0;
    std::string token;

    while ((end_pos = new_line.find(' ', start_pos)) != std::string::npos)
    {
        token = new_line.substr(start_pos, end_pos - start_pos);
        seeds.push_back(std::stoll(token));
        start_pos = end_pos + 1;
    }

    seeds.push_back(std::stoll(new_line.substr(start_pos)));
    return seeds;
}

std::vector<seed_pair> get_seed_pairs(const std::string& line)
{
    std::vector<seed_pair> pairs;

    size_t index = line.find_first_of("0123456789");
    std::string new_line = line.substr(index);

    std::string token;

    std::vector<std::string> split = jumi::split(new_line, ' ');

    for (size_t i = 0; i < split.size(); i += 2)
    {
        seed_pair pair;
        pair.seed_start = std::stoll(split[i]);
        pair.seed_range = std::stoll(split[i + 1]);
        pairs.emplace_back(pair);
    }

    return pairs;
}

std::vector<destination_source_map> get_destination_source_map(const std::vector<std::string>& lines, const std::string& start_delim)
{
    std::vector<destination_source_map> map;
    size_t start_index = 0;
    size_t end_index = 0;

    // Loop to find our start and end indices for each map
    for (size_t i = 0; i < lines.size(); ++i)
    {
        if (lines[i].starts_with(start_delim))
        {
            start_index = i + 1;
        }
    }
    for (size_t i = start_index; i < lines.size(); ++i)
    {
        if (lines[i].empty())
        {
            end_index = i;
            break;
        }
    }

    for (size_t i = start_index; i < end_index; ++i)
    {
        destination_source_map dsm;
        std::vector<std::string> split = jumi::split(lines[i], ' ');
        dsm.destination_range_start = std::stoll(split[0]);
        dsm.source_range_start = std::stoll(split[1]);
        dsm.range = std::stoll(split[2]);
        map.emplace_back(dsm);
    }

    return map;
}

int64 produce_destination(int64 source, const destination_source_map& dsm)
{
    if (source >= dsm.source_range_start)
    {
        if (source < dsm.source_range_start + dsm.range)
        {
            int64 offset = source - dsm.source_range_start;
            int64 destination = dsm.destination_range_start + offset;
            return destination;
        }
    }
    return source;
}

int64 process_seed(int64 seed_number, const std::vector<std::vector<destination_source_map>>& pathways)
{
    int64 dest = seed_number;

    for (const std::vector<destination_source_map>& path : pathways)
    {
        for (const destination_source_map& dsm : path)
        {
            int64 old_dest = dest;
            dest = produce_destination(dest, dsm);
            if (dest != old_dest)
            {
                break;
            }
        }
    }

    return dest;
}

std::vector<int64> process_seeds(const std::vector<int64>& seeds, const std::vector<std::vector<destination_source_map>>& pathways)
{
    std::vector<int64> results;

    for (int64 seed : seeds)
    {
        results.emplace_back(process_seed(seed, pathways));
    }

    return results;
}

std::vector<int64> process_seed_pairs(const std::vector<seed_pair>& seed_pairs, const std::vector<std::vector<destination_source_map>>& pathways)
{
    std::vector<int64> results;

    for (const seed_pair& pair : seed_pairs)
    {
        size_t last_seed = pair.seed_start + pair.seed_range;
        for (size_t i = pair.seed_start; i < last_seed; ++i)
        {
            results.emplace_back(process_seed(i, pathways));
        }
    }

    return results;
}

int64 find_lowest(const std::vector<int64>& numbers)
{
    int64 lowest = std::numeric_limits<int>::max();
    for (int64 num : numbers)
    {
        if (num < lowest)
        {
            lowest = num;
        }
    }
    return lowest;
}

int main()
{
    // ----------------------------------------
    // Part 1
    // ----------------------------------------
    std::fstream file = jumi::open_file(filepath);
    std::stringstream ss;
    ss << file.rdbuf();
    std::string contents = ss.str();
    std::vector<std::string> lines = jumi::split(contents, '\n');

    std::vector<int64> seeds = get_seeds(lines[0]);
    std::vector<destination_source_map> seed_to_soil = get_destination_source_map(lines, seed_to_soil_delim);
    std::vector<destination_source_map> soil_to_fertilizer = get_destination_source_map(lines, soil_to_fertilizer_delim);
    std::vector<destination_source_map> fertilizer_to_water = get_destination_source_map(lines, fertilizer_to_water_delim);
    std::vector<destination_source_map> water_to_light = get_destination_source_map(lines, water_to_light_delim);
    std::vector<destination_source_map> light_to_temperature = get_destination_source_map(lines, light_to_temperature_delim);
    std::vector<destination_source_map> temperature_to_humidity = get_destination_source_map(lines, temperature_to_humidity_delim);
    std::vector<destination_source_map> humidity_to_location = get_destination_source_map(lines, humidity_to_location_delim);
    std::vector<std::vector<destination_source_map>> pathways;
    pathways.push_back(seed_to_soil);
    pathways.push_back(soil_to_fertilizer);
    pathways.push_back(fertilizer_to_water);
    pathways.push_back(water_to_light);
    pathways.push_back(light_to_temperature);
    pathways.push_back(temperature_to_humidity);
    pathways.push_back(humidity_to_location);
    std::vector<int64> results = process_seeds(seeds, pathways);
    int64 lowest = std::numeric_limits<int64>::max();
    // Find the lowest number in the results vector
    for (int64 result : results)
    {
        if (result < lowest)
        {
            lowest = result;
        }
    }
    std::cout << "Lowest result part 1: " << lowest << '\n';

    // ----------------------------------------
    // Part 2
    // ----------------------------------------

    std::vector<seed_pair> seed_pairs = get_seed_pairs(lines[0]);
    std::vector<int64> results2 = process_seed_pairs(seed_pairs, pathways);
    lowest = std::numeric_limits<int64>::max();
    lowest = find_lowest(results2);

    std::cout << "Lowest result part 2: " << lowest << '\n';

    return 0;
}
