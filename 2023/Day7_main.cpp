#include "jumi_utils.h"
#include <algorithm>
#include <string>
#include <unordered_map>
#include <stdexcept>

/* #define DEBUG */

constexpr const char* filepath{ "Day7_input.txt" };

using jumi::operator<<;

enum class sln_part
{
    part_one,
    part_two,
};

struct hand
{
    std::string hand;
    int64 bid_amount;

    std::string to_string() const
    {
        return std::string(hand + "|" + std::to_string(bid_amount));
    }
};

enum hand_strength
{
    high_card,
    one_pair,
    two_pair,
    three_of_a_kind,
    full_house,
    four_of_a_kind,
    five_of_a_kind,
    INVALID_STRENGTH
};

const std::unordered_map<std::string, int32> card_strength_map =
{
    { "2",   2 },
    { "3",   3 },
    { "4",   4 },
    { "5",   5 },
    { "6",   6 },
    { "7",   7 },
    { "8",   8 },
    { "9",   9 },
    { "T",  10 },
    { "J",  11 },
    { "Q",  12 },
    { "K",  13 },
    { "A",  14 },
};

std::ostream& operator<<(std::ostream& os, const hand& hand)
{
    os << '(' << hand.hand << ',' << hand.bid_amount << ')';
    return os;
}

std::vector<hand> parse_input(std::fstream& file)
{
    std::vector<hand> hands;
    std::string line;

    while (std::getline(file, line))
    {
        std::vector<std::string> split = jumi::split(line, ' ');
        hands.emplace_back(split[0], std::stoll(split[1]));
    }

    return hands;
}

hand_strength calculate_hand_strength(const hand& hand, sln_part part = sln_part::part_one)
{
    std::unordered_map<char, int32> card_count;
    hand_strength strength = INVALID_STRENGTH;

    for (char c : hand.hand)
    {
        card_count[c] += 1;
    }

    if (part == sln_part::part_two)
    {
        int32 joker_count = 0;
        auto it = card_count.find('J');
        if (it != card_count.end())
        {
            joker_count = it->second;
            it->second = 0;
        }

        char key;
        int32 highest_count = 0;
        for (const std::pair<const char, int32> card : card_count)
        {
            if (card.first != 'J' && card.second > highest_count)
            {
                key = card.first;
                highest_count = card.second;
            }
        }
        card_count[key] += joker_count;
    }

    std::vector<int32> counts;
    for (const std::pair<const char, int32>& pair : card_count)
    {
        if (pair.second > 0)
        {
            counts.push_back(pair.second);
        }
    }

    std::sort(counts.begin(), counts.end(), std::greater<int32>());

    std::string hand_str = hand.to_string() + "  ";
    if (counts[0] == 5)
    {
        hand_str += "Five of a kind";
        strength = five_of_a_kind;
    }
    else if (counts[0] == 4)
    {
        hand_str += "Four of a kind";
        strength = four_of_a_kind;
    }
    else if (counts[0] == 3)
    {
        if (counts.size() > 1 && counts[1] == 2)
        {
            hand_str += "Full house";
            strength = full_house;
        }
        else
        {
            hand_str += "Three of a kind";
            strength = three_of_a_kind;
        }
    }
    else if (counts[0] == 2)
    {
        if (counts.size() > 1 && counts[1] == 2)
        {
            hand_str += "Two pair";
            strength = two_pair;
        }
        else
        {
            hand_str += "One pair";
            strength = one_pair;
        }
    }
    else
    {
        hand_str += "High card";
        strength = high_card;
    }

#ifdef DEBUG
    std::cout << hand_str << '\n';
#endif

    return strength;
}

bool compare_hands(const hand& hand_a, const hand& hand_b)
{
    // This function will return false if hand_a is the greater hand over hand_b, else true
    hand_strength strength_a = calculate_hand_strength(hand_a);
    hand_strength strength_b = calculate_hand_strength(hand_b);

    if (strength_a != strength_b)
    {
        std::cerr << "Cannot compare two hands of different hand strengths: " << hand_a << " to " << hand_b << '\n';
        return true;
    }

    for (size_t i = 0; i < hand_a.hand.size(); ++i)
    {
        std::string card_a;
        std::string card_b;
        card_a = hand_a.hand[i];
        card_b = hand_b.hand[i];

        std::cerr << "Checking " << card_a << " against " << card_b << '\n';

        if (card_strength_map.at(card_a) < card_strength_map.at(card_b))
        {
            return true;
        }
        else if (card_strength_map.at(card_a) > card_strength_map.at(card_b))
        {
            return false;
        }
    }

    std::cout << "Two hands have the same values: [" << hand_a << " | " << hand_b << "] true will be returned\n";
    return true;
}

bool compare_hands_2(const hand& hand_a, const hand& hand_b)
{
    // This function will return false if hand_a is the greater hand over hand_b, else true
    hand_strength strength_a = calculate_hand_strength(hand_a, sln_part::part_two);
    hand_strength strength_b = calculate_hand_strength(hand_b, sln_part::part_two);

    if (strength_a != strength_b)
    {
        std::cerr << "Cannot compare two hands of different hand strengths: " << hand_a << " to " << hand_b << '\n';
        return true;
    }

    for (size_t i = 0; i < hand_a.hand.size(); ++i)
    {
        std::string card_a;
        std::string card_b;
        card_a = hand_a.hand[i];
        card_b = hand_b.hand[i];
        std::cout << "Checking " << card_a << " against " << card_b << '\n';

        int32 card_a_strength = card_strength_map.at(card_a);
        int32 card_b_strength = card_strength_map.at(card_b);

        if (card_a_strength == 11) card_a_strength = 1;
        if (card_b_strength == 11) card_b_strength = 1;

        if (card_a_strength < card_b_strength)
        {
            return true;
        }
        else if (card_a_strength > card_b_strength)
        {
            return false;
        }
    }

    std::cout << "Two hands have the same values: [" << hand_a << " | " << hand_b << "] true will be returned\n";
    return true;
}

std::unordered_map<hand_strength, std::vector<hand>> parse_hands(const std::vector<hand>& hands, sln_part part = sln_part::part_one)
{
    std::unordered_map<hand_strength, std::vector<hand>> hand_map;
    std::vector<hand> high_cards;
    std::vector<hand> one_pairs;
    std::vector<hand> two_pairs;
    std::vector<hand> three_of_a_kinds;
    std::vector<hand> full_houses;
    std::vector<hand> four_of_a_kinds;
    std::vector<hand> five_of_a_kinds;

    for (const hand& hand : hands)
    {
        hand_strength strength = calculate_hand_strength(hand, part);
        switch (strength)
        {
            case high_card:
            {
                high_cards.push_back(hand);
                break;
            }
            case one_pair:
            {
                one_pairs.push_back(hand);
                break;
            }
            case two_pair:
            {
                two_pairs.push_back(hand);
                break;
            }
            case three_of_a_kind:
            {
                three_of_a_kinds.push_back(hand);
                break;
            }
            case full_house:
            {
                full_houses.push_back(hand);
                break;
            }
            case four_of_a_kind:
            {
                four_of_a_kinds.push_back(hand);
                break;
            }
            case five_of_a_kind:
            {
                five_of_a_kinds.push_back(hand);
                break;
            }
            case INVALID_STRENGTH:
            {
                throw std::runtime_error("Something is wrong");
                break;
            }
        }
    }

    hand_map[high_card]       = std::move(high_cards);
    hand_map[one_pair]        = std::move(one_pairs);
    hand_map[two_pair]        = std::move(two_pairs);
    hand_map[three_of_a_kind] = std::move(three_of_a_kinds);
    hand_map[full_house]      = std::move(full_houses);
    hand_map[four_of_a_kind]  = std::move(four_of_a_kinds);
    hand_map[five_of_a_kind]  = std::move(five_of_a_kinds);

    return hand_map;
}

int64 get_card_bid(const hand& hand, sln_part part = sln_part::part_one)
{
    static int64 card_rank = 1;
    static int64 card_rank_2 = 1;

    int64 total_bid = 0;

    if (part == sln_part::part_one)
    {
        total_bid = card_rank * hand.bid_amount;
        ++card_rank;
    }
    else if (part == sln_part::part_two)
    {
        std::cerr << "Assigning " << card_rank_2 << " to " << hand << '\n';
        total_bid = card_rank_2 * hand.bid_amount;
        ++card_rank_2;
    }
    return total_bid;
}

int64 sum_card_list(const std::vector<hand>& card_list, sln_part part = sln_part::part_one)
{
    int64 total_sum = 0;

    for (const hand& hand : card_list)
    {
        total_sum += get_card_bid(hand, part);
    }
    return total_sum;
}

int64 calculate_card_bids(std::unordered_map<hand_strength, std::vector<hand>>& hand_strength_map, sln_part part = sln_part::part_one)
{
    int64 total_sum = 0;

    std::vector<hand>& curr_hands = hand_strength_map[high_card];
    total_sum += sum_card_list(curr_hands, part);
    curr_hands = hand_strength_map[one_pair];
    total_sum += sum_card_list(curr_hands, part);
    curr_hands = hand_strength_map[two_pair];
    total_sum += sum_card_list(curr_hands, part);
    curr_hands = hand_strength_map[three_of_a_kind];
    total_sum += sum_card_list(curr_hands, part);
    curr_hands = hand_strength_map[full_house];
    total_sum += sum_card_list(curr_hands, part);
    curr_hands = hand_strength_map[four_of_a_kind];
    total_sum += sum_card_list(curr_hands, part);
    curr_hands = hand_strength_map[five_of_a_kind];
    total_sum += sum_card_list(curr_hands, part);

    return total_sum;
}

int main()
{
    // ----------------------------------------
    // Part One
    // ----------------------------------------
    std::fstream file = jumi::open_file(filepath);
    std::vector<hand> hands = parse_input(file);
    std::unordered_map<hand_strength, std::vector<hand>> hand_strength_map { parse_hands(hands) };

    for (std::pair<const hand_strength, std::vector<hand>>& pair : hand_strength_map)
    {
        std::sort(pair.second.begin(), pair.second.end(), compare_hands);
    }

    int64 part_one_result = calculate_card_bids(hand_strength_map);
    std::cout << "Part1 result: " << part_one_result << '\n';

    // ----------------------------------------
    // Part Two
    // ----------------------------------------

    std::unordered_map<hand_strength, std::vector<hand>> hand_strength_map_2 { parse_hands(hands, sln_part::part_two) };

    for (std::pair<const hand_strength, std::vector<hand>>& pair : hand_strength_map_2)
    {
        std::sort(pair.second.begin(), pair.second.end(), compare_hands_2);
    }

    int64 part_two_result = calculate_card_bids(hand_strength_map_2, sln_part::part_two);
    std::cout << "Part2 result: " << part_two_result << '\n';

    return 0;
}
