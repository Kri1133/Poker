#pragma once
#include <string>
#include <vector>

class Player
{
    int playerNumber;
    bool goodCombination = false;
    std::string name;
    std::vector<std::vector<std::string>> hand; // Each player gets 2 cards
    std::string action;

public:
    void setNumber(const int i);
    int getNumber();
    void setName();
    void setAction();
    std::string getName();
    void addCardToHand(std::vector<std::string> card);
    std::vector<std::vector<std::string>> getHand();
};