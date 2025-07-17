#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "player.h"
#include "combination_check.h"
using namespace std;

const int PLAYER_COUNT = 4;

 /*Player count must not be > 10
 Otherwise bad things will happen
 Or so I was told*/

std::vector<std::vector<string>> shuffleCards(string Deck[52][2])
{
	bool isCardUsed[52];
	for (int i = 0; i < 52; i++)
	{
		isCardUsed[i] = 0;
	}
	std::vector<std::vector<string>> shuffledDeck(52, std::vector<string>(2));
	srand(time(0));
	int randomNum = rand() % 52;
	for (int i = 0; i < 52; i++)
	{
		randomNum = rand() % 52;
		if (!isCardUsed[randomNum])
		{
			isCardUsed[randomNum] = 1;
			shuffledDeck[i][0] = Deck[randomNum][0];
			shuffledDeck[i][1] = Deck[randomNum][1];
		}
		else
		{
			while (isCardUsed[randomNum] == 1)
			{
				randomNum = rand() % 52;
			}
			isCardUsed[randomNum] = 1;
			shuffledDeck[i][0] = Deck[randomNum][0];
			shuffledDeck[i][1] = Deck[randomNum][1];
		}
	}

	return shuffledDeck;
}

void dealingCards(std::vector<Player>& players, vector<vector<string>>& deck)
{
	int cardIndex = 0;
	for (int i = 0; i < PLAYER_COUNT; i++)
	{
		players[i].addCardToHand(deck[cardIndex]);
		cardIndex++;
		players[i].addCardToHand(deck[cardIndex]);
		cardIndex++;
	}
}

bool checkCombinations(vector<vector<string>>& comCards, vector<vector<string>> hand)
{
	bool goodHand = false;
	/*hand.insert(hand.end(), comCards.begin(), comCards.end());
	std::sort(hand.begin(), hand.end());
	int counter = 0;
	do
	{
		checkPermutation();
	} while (std::next_permutation(hand.begin(), hand.end()));
	std::cout << counter;*/
	return goodHand;
}


int main()
{
	std::cout << "Hello World" << endl;
	string CardNames[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace" };
	int CardNamesSize = sizeof(CardNames) / sizeof(CardNames[0]); // sizeof() gives the size in bytes
	string Suits[] = { "Hearts", "Diamonds", "Clubs", "Spades" };
	int SuitsSize = 4;
	string Deck[52][2];
	int counter = 0;

	for (int i = 0; i < CardNamesSize; i++)
	{
		for (int j = 0; j < SuitsSize; j++)
		{
			Deck[counter][0] = CardNames[i];
			Deck[counter][1] = Suits[j];
			counter++;
		}
	}

	vector<Player> players(PLAYER_COUNT);
	for (int i = 0; i < PLAYER_COUNT; i++)
	{
		players[i].setNumber(i + 1);
		players[i].setName();
	}

	std::vector<std::vector<string>> shuffledDeck = shuffleCards(Deck);

	dealingCards(players, shuffledDeck);

	for (int i = 0; i < PLAYER_COUNT; i++)
	{
		std::cout << players[i].getName() << ": " << endl;
		std::vector<std::vector<std::string>> hand = players[i].getHand();
		for (int j = 0; j < 2; j++)
		{
			std::cout << hand[j][0] << " of " << hand[j][1] << endl;
		}
	}

	shuffledDeck.erase(shuffledDeck.begin(), shuffledDeck.begin() + 2 * PLAYER_COUNT); // remove used cards

	std::vector<std::vector<string>> communityCards;

	// Dealing the flop
	for (int i = 0; i < 3; i++)
	{
		communityCards.push_back(shuffledDeck[i]);
	}

	shuffledDeck.erase(shuffledDeck.begin(), shuffledDeck.begin() + 3);

	double bet = 0;

	for (int i = 0; i < PLAYER_COUNT - 1; i++)
	{
		vector<vector<string>> cardsToPass = players[i].getHand();
		checkCombinations(communityCards, cardsToPass);
		// TODO add check cards method
	}

	// Dealing the turn
	communityCards.push_back(shuffledDeck[0]);

	shuffledDeck.erase(shuffledDeck.begin());

	// Dealing the river
	communityCards.push_back(shuffledDeck[0]);

	shuffledDeck.erase(shuffledDeck.begin());

	return 0;
}