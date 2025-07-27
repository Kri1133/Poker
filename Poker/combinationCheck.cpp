#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "combinationCheck.h"
#include "globals.h"

// In this file hand represents the concatenated vectors of the player's hand
// and the community cards

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
	int totalPairs = 0;
	for (int i = 0; i < 2; i++) { // Only the first two cards are from the player's hand
		for (int j = 2; j < hand.size(); j++) {
			if (hand[i][0] == hand[j][0]) {
				totalPairs++;
			}
		}
	}
	return totalPairs;
};
// FUNCTION IS COMPLETED

int getValueFromRank(const std::string& rank) {
	for (const auto& pair : rankMap) {
		if (pair.second == rank) {
			return pair.first;
		}
	}
}

bool isGoodStartingHand(const std::vector<std::vector<std::string>>& hand)
{
	const std::string& rank1 = hand[0][0];
	const std::string& suit1 = hand[0][1];
	const std::string& rank2 = hand[1][0];
	const std::string& suit2 = hand[1][1];

	int v1 = getValueFromRank(rank1);
	int v2 = getValueFromRank(rank2);

	// Pair of any value is good
	if (rank1 == rank2) return true;

	// Both high cards (10 or higher)
	if (v1 >= 10 && v2 >= 10) return true;

	// Suited connectors 10 or above
	if (suit1 == suit2 && std::abs(v1 - v2) == 1 && (v1 >= 10 || v2 >= 10)) return true;

	return false;
}

std::string getCombinationName(const std::vector<std::vector<std::string>>& hand) {
	std::vector<std::vector<std::string>> bruh = hand;
	if (isRoyalFlush(bruh))    return "Royal Flush";
	if (isStraight(bruh) && isFlush(bruh)) return "Straight Flush";
	if (isFourOfaKind(bruh))   return "Four of a Kind";
	if (isFullHouse(bruh))     return "Full House";
	if (isFlush(bruh))         return "Flush";
	if (isStraight(bruh))      return "Straight";
	if (isThreeOfaKind(bruh))  return "Three of a Kind";
	if (isPair(bruh) == 2)     return "Two Pair";
	if (isPair(bruh) == 1)     return "One Pair";
	return "High Card";
}