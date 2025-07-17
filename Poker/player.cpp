#include <iostream>
#include <string>
#include "player.h"

void Player::setName()
{
    std::cout << "Enter the name of the player: ";
    std::getline(std::cin, name);
}

int Player::getNumber()
{
    return playerNumber;
}

void Player::setNumber(int i)
{
    playerNumber = i;
}

std::string Player::getName()
{
    return name;
}

void Player::addCardToHand(std::vector<std::string> card)
{
    hand.push_back(card);
}

std::vector<std::vector<std::string>> Player::getHand() {
    return hand;
}