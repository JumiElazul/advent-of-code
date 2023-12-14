#include "jumi_utils.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

const char* filepath { "Day4_input.txt" };

struct Scratchcard
{
    int cardNumber{};
    std::vector<std::string> winningNumbers;
    std::vector<std::string> gameNumbers;

    bool operator<(const Scratchcard& other) const { return cardNumber < other.cardNumber; }
    bool operator>(const Scratchcard& other) const { return cardNumber > other.cardNumber; }
    bool operator==(const Scratchcard& other) const { return cardNumber == other.cardNumber; }
};

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vec)
{
    for (const auto& str : vec)
    {
        std::cout << str << ' ';
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const Scratchcard& scratchcard)
{
    os << "Card " << scratchcard.cardNumber << ": ";
    for (const std::string& str : scratchcard.winningNumbers)
    {
        os << str << ',';
    }
    os << "|";

    for (const std::string& str : scratchcard.gameNumbers)
    {
        os << str << ',';
    }

    return os;
}

std::vector<std::string> GetWinningNumbers(const std::string& line)
{
    std::vector<std::string> split = jumi::split(line, '|');
    std::string numbers = split[0];
    std::vector<std::string> split2 = jumi::split(numbers, ':');
    numbers = split2[1];
    jumi::trim(numbers);

    std::string newNumbers;

    for (size_t i = 0; i < numbers.length(); ++i)
    {
        newNumbers += numbers[i];

        if (numbers[i] == ' ' && numbers[i + 1] == ' ')
        {
            // Skip the next whitespace
            ++i;
        }
    }

    std::vector<std::string> winningNumbers = jumi::split(newNumbers, ' ');

    return winningNumbers;
}

std::vector<std::string> GetGameNumbers(const std::string& line)
{
    std::vector<std::string> split = jumi::split(line, '|');
    std::string numbers = split[1];
    numbers = jumi::trim(numbers);

    std::string newNumbers;

    for (size_t i = 0; i < numbers.length(); ++i)
    {
        newNumbers += numbers[i];

        if (numbers[i] == ' ' && numbers[i + 1] == ' ')
        {
            // Skip the next whitespace
            ++i;
        }
    }

    std::vector<std::string> gameNumbers = jumi::split(newNumbers, ' ');

    return gameNumbers;
}

int CountWinsCard(const Scratchcard& card)
{
    int winCount{};

    std::vector<std::string> winningNumbers = card.winningNumbers;
    std::vector<std::string> gameNumbers = card.gameNumbers;

    for (const std::string& number : gameNumbers)
    {
        auto find = std::find(winningNumbers.begin(), winningNumbers.end(), number);

        if (find != winningNumbers.end())
        {
            ++winCount;
        }
    }

    return winCount;
}

long long CountWins(const std::map<Scratchcard, long long>& cardMap)
{
    long long totalScore{};

    for (const std::pair<const Scratchcard, long long>& card : cardMap)
    {
        long long numOfWins{};
        for (const std::string& str : card.first.gameNumbers)
        {
            auto find = std::find(card.first.winningNumbers.begin(), card.first.winningNumbers.end(), str);

            if (find != card.first.winningNumbers.end())
            {
                ++numOfWins;
            }
        }

        long long score{ 1 };
        if (numOfWins == 0)
        {
            continue;
        }
        else
        {
            while (numOfWins > 1)
            {
                score *= 2;
                --numOfWins;
            }
            totalScore += score;
        }
    }

    return totalScore;
}

int GetCardNumber(const std::string& line)
{
    std::vector<std::string> split = jumi::split(line, '|');
    split = jumi::split(split[0], ':');
    std::string card = split[0];

    int first{};
    int last = card.find_last_of(' ');
    for (size_t i = 0; i < card.length(); ++i)
    {
        if (std::isdigit(line[i]))
        {
            first = i;
            break;
        }
    }

    return std::stoi(card.substr(first, last));
}

std::map<Scratchcard, int> ProcessScratchcards(std::map<Scratchcard, int>& scratchcards, int totalCardNumbers)
{
    std::map<Scratchcard, int> additionalCards;

    for (const std::pair<const Scratchcard, int>& pair : scratchcards)
    {
        const Scratchcard& card = pair.first;
        int count = pair.second;

        int winCount = CountWinsCard(card);

        for (int i = 0; i < winCount; ++i)
        {
            Scratchcard newCard = card;
            newCard.cardNumber += i;

            if (newCard.cardNumber <= totalCardNumbers) 
            {
                additionalCards[newCard] += winCount * count;
            }
        }
    }

    for (const std::pair<const Scratchcard, int>& card : additionalCards)
    {
        if (scratchcards.find(card.first) != scratchcards.end()) 
        {
            scratchcards[card.first] += card.second;
        }
    }

    return scratchcards;
}

std::vector<std::string> vectorize_input(std::fstream& file)
{
    std::vector<std::string> vector;
    std::string line_str;

    while (std::getline(file, line_str))
    {
        vector.push_back(line_str);
    }

    return vector;
}

int main()
{
    std::fstream file = jumi::open_file(filepath);
    std::vector<std::string> lines = vectorize_input(file);

    std::map<Scratchcard, long long> cardMap;

    for (const std::string& line : lines)
    {
        Scratchcard card;
        card.winningNumbers = GetWinningNumbers(line);
        card.gameNumbers = GetGameNumbers(line);
        card.cardNumber = GetCardNumber(line);
        cardMap[card] = 1;
    }

    long long totalscore = CountWins(cardMap);
    std::cout << "totalscore: " << totalscore << '\n';

    for (const std::pair<const Scratchcard, long long>& pair : cardMap)
    {
        long long wins = CountWinsCard(pair.first);
        long long addCount{ wins * cardMap[pair.first] };

        for (int i = 1; i <= wins; ++i)
        {
            Scratchcard card;
            card.cardNumber = pair.first.cardNumber + i;
            cardMap[card] += addCount;
        }
    }

    long long sum{};
    for (const std::pair<const Scratchcard, const int>& pair : cardMap)
    {
        sum += pair.second;
    }

    std::cout << "Sum of total cards: " << sum << '\n';

    return 0;
}
