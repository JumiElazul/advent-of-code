#include "jumi_utils.h"
#include <ios>
#include <iostream>
#include <sstream>
#include <openssl/md5.h>

// --- Day 4: The Ideal Stocking Stuffer ---
// Santa needs help mining some AdventCoins (very similar to bitcoins) to use as gifts for all the economically forward-thinking 
// little girls and boys.

// To do this, he needs to find MD5 hashes which, in hexadecimal, start with at least five zeroes. The input to the MD5 hash is 
// some secret key (your puzzle input, given below) followed by a number in decimal. To mine AdventCoins, you must find Santa the 
// lowest positive number (no leading zeroes: 1, 2, 3, ...) that produces such a hash.

// For example:

// If your secret key is abcdef, the answer is 609043, because the MD5 hash of abcdef609043 starts with five zeroes (000001dbbfa...), 
// and it is the lowest such number to do so.
// If your secret key is pqrstuv, the lowest number it combines with to make an MD5 hash starting with five zeroes is 1048970; that is, 
// the MD5 hash of pqrstuv1048970 looks like 000006136ef....

// --- Part Two ---
// Now find one that starts with six zeroes.

const std::string part_one_prefix = "00000";
const std::string part_two_prefix = "000000";

std::string calculate_MD5(const std::string& input)
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)input.c_str(), input.length(), (unsigned char*)&digest);

    std::stringstream ss;
    for (size_t i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return ss.str();
}

void part_one()
{
    std::fstream file{ jumi::open_file("Day4_input.txt") };
    std::string contents{ jumi::stringify_file(file) };
    contents = jumi::trim(contents);
    std::string md5;

    constexpr int end = 1000000000;
    for (size_t i = 0; i < end; ++i)
    {
        std::string new_value = contents + std::to_string(i);
        md5 = calculate_MD5(new_value);

        if (md5.starts_with(part_one_prefix))
        {
            std::cout << "The lowest number to produce a hash with 5 leading zeroes is: " << i << "\n";
            break;
        }
    }
}

void part_two()
{
    std::fstream file{ jumi::open_file("Day4_input.txt") };
    std::string contents{ jumi::stringify_file(file) };
    contents = jumi::trim(contents);
    std::string md5;

    constexpr int end = 1000000000;
    for (size_t i = 0; i < end; ++i)
    {
        std::string new_value = contents + std::to_string(i);
        md5 = calculate_MD5(new_value);

        if (md5.starts_with(part_two_prefix))
        {
            std::cout << "The lowest number to produce a hash with 6 leading zeroes is: " << i << "\n";
            break;
        }
    }
}

int main()
{
    part_one();
    part_two();
}
