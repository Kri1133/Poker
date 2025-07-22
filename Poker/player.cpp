#include <iostream>
#include <string>
#include "player.h"

void Player::setName()
{
	std::cout << "Enter the name of the player: ";
	std::getline(std::cin, name);
}

std::string Player::getName()
{
	return name;
}

void Player::setNumber(int i)
{
	playerNumber = i;
}

int Player::getNumber()
{
	return playerNumber;
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
	std::cout << "Your move (Fold/Bet/Raise/Call/AllIn): ";
	std::string action;
	std::cin >> action;
	if (action == "Fold") {
		return Action::Fold;
	}
	else if (action == "Bet") {
		Player::bet();
		return Action::Bet;
	}
	else if (action == "Raise") {
		Player::raise();
		return Action::Raise;
	}
	else if (action == "Call") {
		Player::call();
		return Action::Call;
	}
	else if (action == "AllIn") {
		return Action::AllIn;
	}
}

void Player::fold()
{
	folded = true;
}

void Player::bet() {
	int betAmount = 0;
	std::cout << "Enter the amount you want to bet: ";
	std::cin >> betAmount;
	if (betAmount > chips) {
		std::cout << "You don't have enough chips to bet that amount!" << std::endl;
		Player::bet(); // Ask for bet again
		return;
	}
	chips -= betAmount;
	currentBet = betAmount;
	pot += betAmount;
	std::cout << "You have bet " << betAmount << std::endl;
}

void Player::call()
{
	if (currentBet >= chips) {
		pot += chips;
		chips = 0;
	}
	else
	{
		chips -= currentBet;
		pot += currentBet;
	}
}

void Player::raise() {
	if (currentBet >= chips) {
		std::cout << "You don't have enough chips to raise!" << std::endl;
		std::cout << "You can only fold or go all in." << std::endl;
		std::string newAction;
		std::cin >> newAction;
		if (newAction == "fold") {
			Player::fold();
			return;
		}
		else if (newAction == "call" || newAction == "allin")
		{
			Player::call();
			return;
		}
	}
	else
	{
		int newAmount = 0;
		std::cout << "I raise the bet to " << newAmount << std::endl;
		std::cin >> newAmount;
		if (newAmount > chips) {
			std::cout << "You don't have enough chips to raise!" << std::endl;
			Player::raise();
			return;
		}
		else if (newAmount <= currentBet) {
			std::cout << "You have to raise more than " << currentBet << "." << std::endl;
			Player::raise();
			return;
		}
		chips -= newAmount;
		currentBet = newAmount;
		pot += newAmount;
	}
}

Action botPlayer::chooseAction()
{
	std::cout << "All In!" << std::endl;
	return Action::Call;
}