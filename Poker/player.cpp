#include <iostream>
#include <string>
#include <map>
#include "player.h"
#include "actionUtils.h"
#include "bettingRounds.h"
#include "globals.h"
#include "combinationCheck.h"


void Player::setName()
{
	name = "Parzival";
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

Action Player::chooseAction(int& currentBet)
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
				bet(currentBet);
				break;
			case Action::raise:
				validAction = true;
				raise(currentBet);
				break;
			case Action::call:
				validAction = true;
				call(currentBet);
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

void Player::bet(int& currentBet) {
	int betAmount = 0;
	std::cout << "Enter the amount you want to bet: ";
	std::cin >> betAmount;
	if (betAmount > chips) {
		std::cout << "You don't have enough chips! You've got " << chips << "." << std::endl;
		Player::bet(currentBet); // Ask for bet again
		return;
	}
	chips -= betAmount;
	currentBet = betAmount;
	std::cout << "You have bet " << betAmount << std::endl;
}

void Player::call(int& currentBet)
{
	if (currentBet >= chips) {
		setIsAllIn();
		chips = 0;
	}
	else
	{
		chips -= currentBet;
	}
}

void Player::raise(int& currentBet) {
	if (currentBet >= chips) {
		std::cout << "You don't have enough chips to raise!" << std::endl;
		std::string newAction;
		std::cin >> newAction;
		if (newAction == "fold") {
			Player::fold();
			folded = true;
			return;
		}
		else if (newAction == "call" || newAction == "allin")
		{
			Player::call(currentBet);
			return;
		}
	}
	else
	{
		int newAmount = 0;
		std::cout << "Enter the amount you want to raise: ";
		std::cin >> newAmount;
		bool enough = false;
		std::cout << "Current bet is " << currentBet << std::endl;
		if (newAmount > currentBet) {
			enough = true;
		}
		while (!enough) {
			if (newAmount > chips) {
				std::cout << "You don't have enough chips to raise!" << std::endl;
				std::cout << "Enter again: ";
				std::cin >> newAmount;
				continue;
			}
			else if (newAmount <= currentBet) {
				std::cout << "You have to raise more than " << currentBet << "." << std::endl;
				std::cout << "Enter again: ";
				std::cin >> newAmount;
				continue;
			}
			enough = true;
		}
		chips -= newAmount;
		currentBet = newAmount;
	}
	bettingDone = false;
}

void botPlayer::raise(int& currentBet) {
	if (currentBet >= chips) {
		std::cout << "All In!!" << std::endl;
		chips = 0;
	}
	else
	{
		srand(time(0));
		int newAmount = ((rand() % 7) + 1) * 100;
		currentBet += newAmount;
		std::cout << name << " raises the bet to " << currentBet << std::endl;
		chips -= newAmount;
	}
	bettingDone = false;
}

void botPlayer::makeManiac() {
	isManiac = true;
};

bool botPlayer::returnIsManiac() {
	return isManiac;
};

std::vector<std::vector<std::string>> Player::getBestFiveCardHand(std::vector<std::vector<std::string>>& communityCards)
{
	// Combine the player's hand and community cards
	std::vector<std::vector<std::string>> combined = hand; // hand is the player's 2 cards
	combined.insert(combined.end(), communityCards.begin(), communityCards.end());

	// Get all possible 5-card combinations
	auto allHands = getAllFiveCardCombinations(combined);

	// Prepare for tie-breaking
	auto getHandStrengthVector = [](const std::vector<std::vector<std::string>>& hand) {
		std::map<std::string, int> counts;
		for (const auto& card : hand)
			counts[card[0]]++;

		std::vector<std::pair<int, int>> grouped;
		for (const auto& [rank, cnt] : counts)
			grouped.push_back({ cnt, rankToValue.at(rank) });

		std::sort(grouped.rbegin(), grouped.rend());

		std::vector<int> result;
		for (const auto& [cnt, val] : grouped)
			for (int i = 0; i < cnt; ++i)
				result.push_back(val);
		return result;
		};

	// Find the best hand (highest combination, then best kickers)
	std::vector<std::vector<std::string>> bestHand;
	int bestRank = 0;
	std::vector<int> bestStrength;
	for (const auto& candidate : allHands) {
		std::string comboName = getCombinationName(candidate);
		int rank = combinationsRank[comboName];
		auto strength = getHandStrengthVector(candidate);

		if (bestHand.empty() || rank > bestRank ||
			(rank == bestRank && strength > bestStrength)) {
			bestHand = candidate;
			bestRank = rank;
			bestStrength = strength;
		}
	}
	return bestHand;
}