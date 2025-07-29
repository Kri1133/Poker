#include <iostream>
#include <string>
#include <map>
#include "player.h"
#include "actionUtils.h"
#include "bettingRounds.h"
#include "globals.h"
#include "combinationCheck.h"

void Player::setName() { name = "Parzival"; }
std::string Player::getName() { return name; }
void Player::setNumber(int i) { playerNumber = i; }
int Player::getNumber() { return playerNumber; }
bool Player::isFolded() { return folded; }
void Player::addCardToHand(std::vector<std::string> card) { hand.push_back(card); }
std::vector<std::vector<std::string>> Player::getHand() { return hand; }
void botPlayer::setBotName(const std::string& newName) { this->name = newName; }

Action Player::chooseAction() {
	std::cout << "Your move (Fold/Bet/Raise/Call): ";
	std::string input;
	std::cin >> input;
	while (true) {
		try {
			Action action = actionFromString(input);
			switch (action) {
			case Action::fold:
				fold();
				return action;
			case Action::bet:
				bet();
				return action;
			case Action::raise:
				raise();
				return action;
			case Action::call:
				call();
				return action;
			}
		}
		catch (const std::invalid_argument&) {
			std::cout << "Invalid action. Please type Fold, Bet, Raise, or Call: ";
			std::cin >> input;
		}
	}
}

void Player::fold() { folded = true; }

void Player::bet() {
	while (true) {
		std::cout << "Enter the amount you want to bet (you have " << chips << " chips): ";
		std::string input;
		std::cin >> input;
		int amount = 0;
		try { amount = std::stoi(input); }
		catch (...) {
			std::cout << "Please enter a valid number." << std::endl;
			continue;
		}
		if (amount > chips) {
			std::cout << "You don't have enough chips to bet that amount!" << std::endl;
		}
		else {
			chips -= amount;
			amountAlreadyPut += amount;
			currentBet = amount;
			break;
		}
	}
}

void Player::call() {
	int toCall = currentBet - amountAlreadyPut;
	if (toCall > chips) toCall = chips;
	chips -= toCall;
	amountAlreadyPut += toCall;
}
// SHOULD WORK

void Player::raise() {
	int toCall = currentBet - amountAlreadyPut;
	if (chips <= toCall) {
		std::cout << "You don't have enough chips to raise! You can only call or fold." << std::endl;
		while (true) {
			std::cout << "Type 'fold' to fold or 'call' to call: ";
			std::string newAction;
			std::cin >> newAction;
			if (newAction == "fold") {
				std::cout << "You folded." << std::endl;
				fold();
				return;
			}
			else if (newAction == "call") {
				call();
				std::cout << "You called the bet of " << currentBet << "." << std::endl;
				return;
			}
			else {
				std::cout << "Invalid action." << std::endl;
			}
		}
	}
	else {
		while (true) {
			std::cout << "Enter the total amount you want to raise to (must be more than " << currentBet
				<< ", no more than " << (chips - amountAlreadyPut) << "), or type 'call' or 'fold': ";
			std::string input;
			std::cin >> input;
			if (input == "fold") {
				std::cout << "You folded." << std::endl;
				fold();
				return;
			}
			else if (input == "call") {
				std::cout << "You called the bet of " << currentBet << "." << std::endl;
				call();
				return;
			}
			int newAmount = 0;
			newAmount = std::stoi(input);
			if (newAmount > chips) {
				std::cout << "You don't have enough chips for that raise!" << std::endl;
			}
			else if (newAmount <= currentBet) {
				std::cout << "You must raise to more than the current bet (" << currentBet << ")." << std::endl;
			}
			else {
				int betDiff = newAmount - amountAlreadyPut;
				chips -= betDiff;
				amountAlreadyPut = newAmount;
				currentBet = newAmount;
				break;
			}
		}
	}
	bettingDone = false;
}
// SHOULD WORK

void botPlayer::raise() {
	if (currentBet >= chips) {
		std::cout << "All In!!" << std::endl;
		amountAlreadyPut += chips;
		chips = 0;
	}
	else {
		srand(time(0));
		int newAmount = ((rand() % 7) + 1) * 100;
		int targetBet = newAmount + currentBet;
		std::cout << name << " raises the bet to " << targetBet << std::endl;
		chips -= newAmount;
		amountAlreadyPut = targetBet;
		currentBet = targetBet;
	}
	bettingDone = false;
}
// SHOULD WORK

void botPlayer::makeManiac() { isManiac = true; }
bool botPlayer::returnIsManiac() { return isManiac; }

std::vector<std::vector<std::string>> Player::getBestFiveCardHand(std::vector<std::vector<std::string>>& communityCards)
{
	std::vector<std::vector<std::string>> combined = hand;
	combined.insert(combined.end(), communityCards.begin(), communityCards.end());

	auto allHands = getAllFiveCardCombinations(combined);

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

	std::vector<std::vector<std::string>> bestHand;
	int bestRank = 0;
	std::vector<int> bestStrength;
	for (const auto& candidate : allHands) {
		std::string comboName = getCombinationName(candidate);
		int rank = combinationsRank[comboName];
		auto strength = getHandStrengthVector(candidate);

		if (bestHand.empty() || rank > bestRank || (rank == bestRank && strength > bestStrength)) {
			bestHand = candidate;
			bestRank = rank;
			bestStrength = strength;
		}
	}
	return bestHand;
}
