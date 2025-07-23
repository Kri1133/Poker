#include <iostream>
#include <string>
#include "player.h"
#include "action_utils.h"

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

Action Player::chooseAction(int& pot)
{
	std::cout << "Your move (Fold/Bet/Raise/Call): ";
	std::string input;
	std::cin >> input;
	try {
		Action action = actionFromString(input);
		switch (action) {
		case Action::fold:
			fold();
			break;
		case Action::bet:
			bet(pot);
			break;
		case Action::raise:
			raise(pot);
			break;
		case Action::call:
			call(pot);
			break;
		}
		return action;
	}
	catch (const std::invalid_argument&) {
		std::cout << "Invalid action. Please type Fold, Bet, Raise, or Call.\n";
	}
}

void Player::fold()
{
	folded = true;
}

void Player::bet(int& pot) {
	int betAmount = 0;
	std::cout << "Enter the amount you want to bet: ";
	std::cin >> betAmount;
	if (betAmount > chips) {
		std::cout << "You don't have enough chips to bet that amount!" << std::endl;
		Player::bet(pot); // Ask for bet again
		return;
	}
	chips -= betAmount;
	currentBet = betAmount;
	pot += betAmount;
	std::cout << "You have bet " << betAmount << std::endl;
}

void Player::call(int& pot)
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

void Player::raise(int& pot) {
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
			Player::call(pot);
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
			Player::raise(pot);
			return;
		}
		else if (newAmount <= currentBet) {
			std::cout << "You have to raise more than " << currentBet << "." << std::endl;
			Player::raise(pot);
			return;
		}
		chips -= newAmount;
		currentBet = newAmount;
		pot += newAmount;
	}
}

void botPlayer::raise(int& pot) {
	if (currentBet >= chips) {
		std::cout << "All In!!" << std::endl;
	}
	else
	{
		int newAmount = 500;
		currentBet += newAmount; 
		std::cout << "I raise the bet to " << currentBet << std::endl;
		chips -= newAmount;
		pot += currentBet;
	}
}

Action botPlayer::chooseAction(int& pot)
{
	std::cout << "All In!" << std::endl;
	return Action::call;
}