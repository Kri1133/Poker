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

void botPlayer::setBotName(const std::string& newName)
{
	this->name = newName;
}

Action Player::chooseAction()
{
    std::cout << "Your move (Fold/Call/Raise/AllIn): ";
    std::string action;
    std::cin >> action;
    if (action == "Fold") return Action::Fold;
    else if (action == "Raise") return Action::Raise;
	else if (action == "Call") return Action::Call;
	else if (action == "AllIn") return Action::AllIn;
}

Action botPlayer::chooseAction()
{
    std::cout << "All In!" << std::endl;
	return Action::Call;
}
