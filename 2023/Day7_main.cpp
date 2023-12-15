#include "jumi_utils.h"
#include <unordered_map>
#include <stdexcept>

constexpr const char* filepath{ "Day7_input.txt" };

using jumi::operator<<;

struct hand
{
    std::string hand;
    int64 bid_amount;
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
    { "3",   3 },
    { "4",   4 },
    { "5",   5 },
    { "6",   6 },
    { "7",   7 },
    { "8",   8 },
    { "9",   9 },
    { "10", 10 },
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

hand_strength calculate_hand_strength(const hand& hand)
{
    std::unordered_map<char, int32> card_count;
    hand_strength strength = INVALID_STRENGTH;

    for (char c : hand.hand)
    {
        card_count[c] += 1;
    }

    std::vector<int32> counts;
    for (const std::pair<const char, int32>& pair : card_count)
    {
        counts.push_back(pair.second);
    }

    std::sort(counts.begin(), counts.end(), std::greater<int32>());

    std::cout << hand << ": ";
    if (counts[0] == 5)
    {
        std::cout << "Five of a kind";
        strength = five_of_a_kind;
    }
    else if (counts[0] == 4)
    {
        std::cout << "Four of a kind";
        strength = four_of_a_kind;
    }
    else if (counts[0] == 3)
    {
        if (counts.size() > 1 && counts[1] == 2)
        {
            std::cout << "Full house";
            strength = full_house;
        }
        else
        {
            std::cout << "Three of a kind";
            strength = three_of_a_kind;
        }
    }
    else if (counts[0] == 2)
    {
        if (counts.size() > 1 && counts[1] == 2)
        {
            std::cout << "Two pair";
            strength = two_pair;
        }
        else
        {
            std::cout << "One pair";
            strength = one_pair;
        }
    }
    else
    {
        std::cout << "High card";
        strength = high_card;
    }
    std::cout << '\n';
    return strength;
}

bool compare_hands(const hand& hand_a, const hand& hand_b)
{
    // This function will return true if hand_a is the greater hand over hand_b, else false
    hand_strength strength_a = calculate_hand_strength(hand_a);
    hand_strength strength_b = calculate_hand_strength(hand_b);

    if (strength_a != strength_b)
    {
        std::cout << "Cannot compare two hands of different hand strengths: " << hand_a << " to " << hand_b << '\n';
        return false;
    }

    for (size_t i = 0; i < hand_a.hand.size(); ++i)
    {
        std::string card_a;
        std::string card_b;
        card_a = hand_a.hand[i];
        card_b = hand_b.hand[i];

        if (card_strength_map.at(card_a) > card_strength_map.at(card_b))
        {
            return true;
        }
        else if (card_strength_map.at(card_a) < card_strength_map.at(card_b))
        {
            return false;
        }
    }

    std::cout << "Two hands have the same values: [" << hand_a << " | " << hand_b << "] false will be returned\n";
    return false;
}

std::unordered_map<hand_strength, std::vector<hand>> parse_hands(const std::vector<hand>& hands)
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
        hand_strength strength = calculate_hand_strength(hand);
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

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<hand> hands = parse_input(file);
    std::unordered_map<hand_strength, std::vector<hand>> hand_strength_map { parse_hands(hands) };

    return 0;
}
