#include <iostream>
#include <string>
#include "player.h"
#include "action_utils.h"
#include "bettingRounds.h"

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

bool Player::isFolded() {
	return folded;
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

Action Player::chooseAction(int& pot, int& currentBet)
{
	std::cout << "Your move (Fold/Bet/Raise/Call): ";
	std::string input;
	std::cin >> input;
	bool validAction = false;
	while (!validAction) {
		try {
			Action action = actionFromString(input);
			switch (action) {
			case Action::fold:
				validAction = true;
				fold();
				break;
			case Action::bet:
				validAction = true;
				bet(pot, currentBet);
				break;
			case Action::raise:
				validAction = true;
				raise(pot, currentBet);
				break;
			case Action::call:
				validAction = true;
				call(pot, currentBet);
				break;
			}
			return action;
		}
		catch (const std::invalid_argument&) {
			std::cout << "Invalid action. Please type Fold, Bet, Raise, or Call: ";
			std::cin >> input;
		}
	}

}

void Player::fold()
{
	folded = true;
}

void Player::bet(int& pot, int& currentBet) {
	int betAmount = 0;
	std::cout << "Enter the amount you want to bet: ";
	std::cin >> betAmount;
	if (betAmount > chips) {
		std::cout << "You don't have enough chips! You've got " << chips << "." << std::endl;
		Player::bet(pot, currentBet); // Ask for bet again
		return;
	}
	chips -= betAmount;
	currentBet = betAmount;
	pot += betAmount;
	std::cout << "You have bet " << betAmount << std::endl;
}

void Player::call(int& pot, int& currentBet)
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

void Player::raise(int& pot, int& currentBet) {
	if (currentBet >= chips) {
		std::cout << "You don't have enough chips to raise!" << std::endl;
		std::cout << "You can only fold or go all in." << std::endl;
		std::string newAction;
		std::cin >> newAction;
		if (newAction == "fold") {
			Player::fold();
			folded = true;
			return;
		}
		else if (newAction == "call" || newAction == "allin")
		{
			Player::call(pot, currentBet);
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
			Player::raise(pot, currentBet);
			return;
		}
		else if (newAmount <= currentBet) {
			std::cout << "You have to raise more than " << currentBet << "." << std::endl;
			Player::raise(pot, currentBet);
			return;
		}
		chips -= newAmount;
		currentBet = newAmount;
		pot += newAmount;
	}
}

void botPlayer::raise(int& pot, int& currentBet) {
	if (currentBet >= chips) {
		std::cout << "All In!!" << std::endl;
		pot += chips; // Add all chips to the pot
		chips = 0;
	}
	else
	{
		srand(time(0));
		int newAmount = ((rand() % 7) + 1)*100;
		currentBet += newAmount;
		std::cout << name << " raises the bet to " << currentBet << std::endl;
		chips -= newAmount;
		pot += currentBet;
		
	}
}

void botPlayer::makeManiac() {
	isManiac = true;
};

bool botPlayer::returnIsManiac() {
	return isManiac;
};