#include "jumi_utils.h"
#include <iostream>

// --- Day 2: I Was Told There Would Be No Math ---
// The elves are running low on wrapping paper, and so they need to submit an order for more. They have a list of the 
// dimensions (length l, width w, and height h) of each present, and only want to order exactly as much as they need.

// Fortunately, every present is a box (a perfect right rectangular prism), which makes calculating the required wrapping 
// paper for each gift a little easier: find the surface area of the box, which is 2*l*w + 2*w*h + 2*h*l. The elves also 
// need a little extra paper for each present: the area of the smallest side.

// For example:

// A present with dimensions 2x3x4 requires 2*6 + 2*12 + 2*8 = 52 square feet of wrapping paper plus 6 square feet of slack,
// for a total of 58 square feet.
// A present with dimensions 1x1x10 requires 2*1 + 2*10 + 2*10 = 42 square feet of wrapping paper plus 1 square foot of slack,
// for a total of 43 square feet.
// All numbers in the elves' list are in feet. How many total square feet of wrapping paper should they order?

//--- Part Two ---
// The elves are also running low on ribbon. Ribbon is all the same width, so they only have to worry about the length they
// need to order, which they would again like to be exact.

// The ribbon required to wrap a present is the shortest distance around its sides, or the smallest perimeter of any one face.
// Each present also requires a bow made out of ribbon as well; the feet of ribbon required for the perfect bow is equal to 
// the cubic feet of volume of the present. Don't ask how they tie the bow, though; they'll never tell.

// For example:

// A present with dimensions 2x3x4 requires 2+2+3+3 = 10 feet of ribbon to wrap the present plus 2*3*4 = 24 feet of ribbon for 
// the bow, for a total of 34 feet.
// A present with dimensions 1x1x10 requires 1+1+1+1 = 4 feet of ribbon to wrap the present plus 1*1*10 = 10 feet of ribbon for 
// the bow, for a total of 14 feet.
// How many total feet of ribbon should they order?

struct dimensions
{
    int width;
    int length;
    int height;
};

std::ostream& operator<<(std::ostream& os, const dimensions& dimensions)
{
    os << dimensions.width << 'x' << dimensions.length << 'x' << dimensions.height;
    return os;
}

dimensions create_dimensions(const std::string& line)
{
    dimensions d{};
    d.width = std::stoi(line.substr(0, line.find_first_of('x')));
    d.length = std::stoi(line.substr(line.find_first_of('x') + 1, line.find_last_of('x') - line.find_first_of('x') - 1));
    d.height = std::stoi(line.substr(line.find_last_of('x') + 1, line.size() - line.find_last_of('x')));
    return d;
}

void part_one()
{
    std::fstream file{ jumi::open_file("Day2_input.txt") };
    std::vector<std::string> lines{ jumi::read_lines(file) };
    auto get_surface_area = [](const dimensions& d) -> int {
        int wl = 2 * (d.width * d.length);
        int wh = 2 * (d.width * d.height);
        int lh = 2 * (d.length * d.height);

        int min = std::min(wl / 2, wh / 2);
        min = std::min(min, lh / 2);

        return wl + wh + lh + min;
    };

    int result = 0;
    for (const std::string& line : lines)
    {
        dimensions d{ create_dimensions(line) };

        int new_area{ get_surface_area(d) };
        result += new_area;
    }

    std::cout << "The total number of square feet needed in wrapping paper: " << result << '\n';
}

void part_two()
{
    std::fstream file{ jumi::open_file("Day2_input.txt") };
    std::vector<std::string> lines{ jumi::read_lines(file) };
    auto get_ribbon_length = [](const dimensions& d) -> int {
        std::array<int, 3> sides = { d.width, d.length, d.height };
        std::sort(sides.begin(), sides.end(), std::less<int>());

        int ribbon_length = (2 * sides[0]) + (2 * sides[1]);
        int extra_ribbon_length = d.width * d.length * d.height;

        return ribbon_length + extra_ribbon_length;
    };

    int result = 0;
    for (const std::string& line : lines)
    {
        dimensions d{ create_dimensions(line) };
        result += get_ribbon_length(d);
    }

    std::cout << "Total number of feet of ribbon needed: " << result << '\n';
}

int main()
{
    part_one();
    part_two();
}
