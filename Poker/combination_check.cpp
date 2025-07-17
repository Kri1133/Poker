#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "combination_check.h"

// In this file hand represents the concatenated vectors of the player's hand
// and the community cards

static std::map<std::string, int> rankMap = {
	{"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9}, {"10", 10}, {"Jack", 11}, {"Queen", 12}, {"King", 13}, {"Ace", 14} };

bool isRoyalFlush(std::vector<std::vector<std::string>>& hand)
{
	std::vector<std::string> royalFlushCards = { "10", "Jack", "Queen", "King", "Ace" };
	for (int i = 0; i < hand.size(); i++)
	{
		for (int j = 0; j < royalFlushCards.size(); j++)
		{
			if (royalFlushCards[j].compare(hand[i][0]))
			{
				royalFlushCards.erase(royalFlushCards.begin() + j);
				if (royalFlushCards.empty())
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool isStraightFlush(std::vector<std::vector<std::string>>& hand)
{
	std::string suitToCheck = hand[0][1];
	for (auto card : hand)
	{
		if (card[1] != suitToCheck)
			return false;
	}
	//  return true;

	sort(hand.begin(), hand.end());
	for (int i = 1; i < hand.size() - 1; i++)
	{ // So that next lines are never out of bounds
		std::string currentCard = hand[i][0];
		std::string previousCard = hand[i - 1][0];
		std::string followingCard = hand[i + 1][0];
		if (rankMap[currentCard] != rankMap[previousCard] + 1 ||
			rankMap[currentCard] != rankMap[followingCard] - 1)
		{
			return false;
		}
	}
	return true;
}

bool isFourOfaKind(std::vector<std::vector<std::string>>& hand)
{
	sort(hand.begin(), hand.end());
	const std::vector<std::vector<std::string>> check1;
	const std::vector<std::vector<std::string>> check2;
	copy(hand.begin(), hand.end() - 1, back_inserter(check1));
	copy(hand.begin() + 1, hand.end(), back_inserter(check1));
	for (int i = 0; i < check1.size() - 1; i++)
	{
		if (check1[i][0] != check1[i + 1][0])
		{
			return false;
		}
	}
	for (int i = 0; i < check1.size() - 1; i++)
	{
		if (check1[i][0] != check1[i + 1][0])
		{
			return false;
		}
	}

};

//bool isFullHouse(std::vector<std::vector<std::string>>& hand) {
//	// TO BE CONTINUED
//};
//
//bool isFlush(std::vector<std::vector<std::string>> hand)
//{
//	std::string suitToCheck = hand[0][1];
//	for (auto card : hand)
//	{
//		if (card[1] != suitToCheck)
//		{
//			return false;
//		}
//	}
//	return true;
//}
//
//bool isStraight(std::vector<std::vector<std::string>>& hand)
//{
//	sort(hand.begin(), hand.end());
//	for (int i = 1; i < hand.size() - 1; i++)
//	{ // So that next lines are never out of bounds
//		std::string currentCard = hand[i][0];
//		std::string previousCard = hand[i - 1][0];
//		std::string followingCard = hand[i + 1][0];
//		if (rankMap[currentCard] != rankMap[previousCard] + 1 ||
//			rankMap[currentCard] != rankMap[followingCard] - 1)
//		{
//			return false;
//		}
//	}
//	return true;
//};
//
//bool isThreeOfaKind(std::vector<std::vector<std::string>>& hand);
//bool isTwoPair(std::vector<std::vector<std::string>>& hand);
//bool isPair(std::vector<std::vector<std::string>>& hand);
//bool isHighCard(std::vector<std::vector<std::string>>& hand);