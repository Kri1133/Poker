#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "combination_check.h"

// In this file hand represents the concatenated vectors of the player's hand
// and the community cards

static std::map<int, std::string> rankMap = { {2, "2"}, {3, "3"}, {4, "4"}, {5, "5"}, {6, "6"}, {7, "7"}, {8, "8"},{9, "9"}, {10, "10"}, {11, "Jack"}, {12, "Queen"}, {13, "King"}, {14, "Ace"} };

bool cmp(std::pair<std::string, int>& a, std::pair<std::string, int>& b)
{
	return a.second < b.second;
}

void sort(std::map<std::string, int>& M)
{
	// Declare vector of pairs 
	std::vector<std::pair<std::string, int> > A;
	// Copy key-value pair from Map 
	// to vector of pairs 
	for (auto& it : M) {
		A.push_back(it);
	}
	// Sort using comparator function 
	sort(A.begin(), A.end(), cmp);
}


bool isRoyalFlush(std::vector<std::vector<std::string>>& hand)
{ // no need to check suits as isFlush() already checks that
	std::vector<std::string> royalFlushCards = { "10", "Jack", "Queen", "King", "Ace" };
	for (int i = 0; i < hand.size(); i++)
	{
		for (int j = 0; j < royalFlushCards.size(); j++)
		{
			if (royalFlushCards[j] == hand[i][0])
			{
				royalFlushCards.erase(royalFlushCards.begin() + j);
				if (royalFlushCards.empty())
				{
					return true;
				}
				continue;
			}
		}
	}
	return false;
}
// FUNCTION IS COMPLETED

bool isFourOfaKind(std::vector<std::vector<std::string>>& hand)
{
	std::map<std::string, int> rankCount;
	for (const auto& card : hand) {
		rankCount[card[0]]++;
	}

	for (const auto& pair : rankCount) {
		if (pair.second == 4) {
			return true;
		}
	}

	return false;
};
// FUNCTION IS COMPLETED

bool isFullHouse(std::vector<std::vector<std::string>>& hand) {
	std::map<std::string, int> rankCount;
	for (const auto& card : hand) {
		rankCount[card[0]]++;
	}

	for (const auto& pair : rankCount) {
		if (rankCount.size() == 2 && (pair.second != 4 || pair.second != 1)) {
			return true;
		}
	}

	return false;
};
// FUNCTION IS COMPLETED

bool isFlush(std::vector<std::vector<std::string>>& hand)
{
	std::string suitToCheck = hand[0][1];
	for (auto card : hand)
	{
		if (card[1] != suitToCheck)
		{
			return false;
		}
	}
	return true;
}
// FUNCTION IS COMPLETED

bool isStraight(std::vector<std::vector<std::string>> hand)
{
	std::vector<std::vector<std::string>> sortedHand;
	// sort(hand.begin(), hand.end()); // Nice try
	for (auto i : rankMap)
	{
		for (int j = 0; j < hand.size(); j++)
			if (i.second == hand[j][0]) {
				sortedHand.push_back(hand[j]);
			}
	}

	for (int i = 1; i < sortedHand.size(); i++)
	{
		std::string currentCard = sortedHand[i][0];
		std::string previousCard = sortedHand[i - 1][0];
		int currValue = 0, prevValue = 0;

		for (const auto& pair : rankMap) {
			if (pair.second == currentCard) {
				currValue = pair.first;
			}
		}

		for (const auto& pair : rankMap) {
			if (pair.second == previousCard) {
				prevValue = pair.first;
			}
		}

		if (currValue != prevValue + 1) {
			return false;
		}
	}
	return true;
};
// FUNCTION IS COMPLETED

bool isThreeOfaKind(std::vector<std::vector<std::string>>& hand) {
	std::map<std::string, int> rankCount;
	for (const auto& card : hand) {
		rankCount[card[0]]++;
	}

	for (const auto& pair : rankCount) {
		if (pair.second == 3) {
			return true;
		}
	}

	return false;
};
// FUNCTION IS COMPLETED

int isPair(std::vector<std::vector<std::string>>& hand) {
	std::map<std::string, int> rankCount;
	for (const auto& card : hand) {
		rankCount[card[0]]++;
	}
	int pairCounter = 0;
	for (const auto& pair : rankCount) {
		if (pair.second == 2) {
			pairCounter++;
		}
	}

	return pairCounter;
};


bool isHighCard(std::vector<std::vector<std::string>>& hand);