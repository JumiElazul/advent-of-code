#include "jumi_utils.h"
#include <unordered_map>
using jumi::operator<<;

constexpr const char* filepath { "Day8_input.txt" };

const std::unordered_map<char, size_t> char_to_index_map
{
    { 'L', 0 },
    { 'R', 1 },
};

struct instructions
{
    std::string instructions;
    size_t index = 0;

    char get_next_instruction()
    {
        char c = instructions[index];
        ++index;

        if (index >= instructions.length())
        {
            index = 0;
        }

        return c;
    }
};

instructions get_instruction_set(const std::vector<std::string>& file_contents)
{
    return { file_contents[0], 0 };
}

std::string get_location(const std::string& line)
{
    return line.substr(0, line.find_first_of(' '));
}

std::vector<std::string> get_values(const std::string& line)
{
    size_t start_index = line.find_first_of('(') + 1;
    size_t end_index = line.find_last_of(')');
    std::string new_line = line.substr(start_index, end_index - start_index);

    return jumi::split(new_line, ", ");
}

std::unordered_map<std::string, std::vector<std::string>> map_input(const std::vector<std::string>& file_contents)
{
    std::unordered_map<std::string, std::vector<std::string>> location_map;

    for (size_t i = 2; i < file_contents.size(); ++i)
    {
        std::string key = get_location(file_contents[i]);
        std::vector<std::string> value = get_values(file_contents[i]);

        location_map[key] = value;
    }

    return location_map;
}

int64 walk_map(const std::unordered_map<std::string, std::vector<std::string>>& map, instructions& instruction_set)
{
    std::string curr_location = "AAA";
    std::string end_location = "ZZZ";
    int64 steps = 0;

    while (curr_location != end_location)
    {
        const std::vector<std::string>& next_locations = map.at(curr_location);
        char direction = instruction_set.get_next_instruction();
        size_t index = char_to_index_map.at(direction);
        curr_location = next_locations.at(index);

        ++steps;
    }

    return steps;
}

int64 walk_map_2(const std::unordered_map<std::string, std::vector<std::string>>& map, instructions& instruction_set, const std::vector<std::string>& start_locations)
{

}

std::vector<std::string> find_start_locations(const std::unordered_map<std::string, std::vector<std::string>>& map)
{
    std::vector<std::string> locations;

    for (const auto& pair : map)
    {
        if (pair.first.ends_with('A'))
        {
            locations.push_back(pair.first);
        }
    }

    return locations;
}

int main()
{
    // ----------------------------------------
    // Part One
    // ----------------------------------------
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> contents = jumi::vectorize_file(file);
    instructions instruction_set = get_instruction_set(contents);
    std::unordered_map<std::string, std::vector<std::string>> location_map = map_input(contents);

    int64 part_one_steps = walk_map(location_map, instruction_set);
    std::cout << "Part1 result: " << part_one_steps << '\n';

    // ----------------------------------------
    // Part Two
    // ----------------------------------------
    instructions instruction_set_2 = get_instruction_set(contents);
    std::vector<std::string> start_locations = find_start_locations(location_map);
    int64 part_two_steps = walk_map_2(location_map, instruction_set_2, start_locations);
    std::cout << "Part2 result: " << part_two_steps << '\n';

    return 0;
}
