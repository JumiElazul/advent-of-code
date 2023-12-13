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

    bool operator<(const Scratchcard& other) const
    {
        return cardNumber < other.cardNumber;
    }
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
        os << str << ' ';
    }
    os << " | ";

    for (const std::string& str : scratchcard.gameNumbers)
    {
        os << str << ' ';
    }

    return os;
}

std::vector<std::string> GetWinningNumbers(const std::string& line)
{
    std::vector<std::string> split = Jumi::Split(line, '|');
    std::string numbers = split[0];
    std::vector<std::string> split2 = Jumi::Split(numbers, ':');
    numbers = split2[1];
    Jumi::Trim(numbers);

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

    std::vector<std::string> winningNumbers = Jumi::Split(newNumbers, ' ');

    return winningNumbers;
}

std::vector<std::string> GetGameNumbers(const std::string& line)
{
    std::vector<std::string> split = Jumi::Split(line, '|');
    std::string numbers = split[1];
    numbers = Jumi::Trim(numbers);

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

    std::vector<std::string> gameNumbers = Jumi::Split(newNumbers, ' ');

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

int CountWins(const std::map<Scratchcard, int>& cardMap)
{
    int totalScore{};

    for (const std::pair<const Scratchcard, int>& card : cardMap)
    {
        int numOfWins{};
        for (const std::string& str : card.first.gameNumbers)
        {
            auto find = std::find(card.first.winningNumbers.begin(), card.first.winningNumbers.end(), str);

            if (find != card.first.winningNumbers.end())
            {
                ++numOfWins;
            }
        }

        int score{ 1 };
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
    std::vector<std::string> split = Jumi::Split(line, '|');
    split = Jumi::Split(split[0], ':');
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

int main()
{
    std::ifstream file(filepath);

    if (!file)
    {
        std::cout << "File with path " << filepath << " could not be opened\n";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string contents = ss.str();

    std::vector<std::string> lines = Jumi::Split(contents, '\n');

    std::map<Scratchcard, int> cardMap;

    for (const std::string& line : lines)
    {
        Scratchcard card;
        std::vector<std::string> winningNumbers = GetWinningNumbers(line);
        std::vector<std::string> gameNumbers = GetGameNumbers(line);
        card.cardNumber = GetCardNumber(line);

        card.winningNumbers = std::move(winningNumbers);
        card.gameNumbers = std::move(gameNumbers);

        cardMap[card] = 1;
    }

    int totalscore = CountWins(cardMap);
    std::cout << "totalscore: " << totalscore << '\n';

    int cardTotal = cardMap.size();
    cardMap = ProcessScratchcards(cardMap, cardTotal);
    int cardCount{};

    for (const std::pair<const Scratchcard, int>& card : cardMap)
    {
        cardCount += card.second;
    }
    std::cout << "Result: " << cardCount << '\n';

    return 0;
}
