#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <memory>
#include <thread>
#include <chrono>
#include "player.h"
#include "hand_combinations.h"
#include "combination_check.h"
#include "action_utils.h"
#include "bettingRounds.h"
#include "globals.h"
using namespace std;

/*Player count must not be > 10
Otherwise bad things will happen
Or so I was told*/

// If you get LNK2019 error rebuild the project(right-click on project and select rebuild)

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

void dealingCards(std::vector<std::unique_ptr<Player>>& players, vector<vector<string>>& deck)
{
	int cardIndex = 0;
	for (int i = 0; i < PLAYER_COUNT; i++)
	{
		players[i]->addCardToHand(deck[cardIndex]);
		cardIndex++;
		players[i]->addCardToHand(deck[cardIndex]);
		cardIndex++;
	}
}

bool checkCombinations(vector<vector<string>> hand)
{
	if (isFlush(hand))
	{

		if (isRoyalFlush(hand)) {
			std::cout << "Royal Flush!" << endl;
			return true;
		}
		else if (isStraight(hand)) {
			std::cout << "Straight Flush!" << endl;
			return true;
		}
		else {
			std::cout << "Flush!" << endl;
			return true;
		}
	}
	else if (isFourOfaKind(hand))
	{
		std::cout << "Four of a Kind!" << endl;
		return true;
	}
	else if (isFullHouse(hand))
	{
		std::cout << "Full House!" << endl;
		return true;
	}
	else if (isStraight(hand))
	{
		std::cout << "Straight!" << endl;
		return true;
	}
	else if (isThreeOfaKind(hand)) // isThreeOfaKind must be checked after isFullHouse
	{
		std::cout << "Three of a Kind!" << endl;
		return true;
	}
	else if (isPair(hand) == 1)
	{
		std::cout << "Pair!" << endl;
		return true;
	}
	else if (isPair(hand) == 2)
	{
		std::cout << "Two Pair!" << endl;
		return true;
	}
	else
	{
		std::cout << "High Card!" << endl;
		return false;
	}
}

void generateManiacs(std::vector<std::unique_ptr<Player>>& players) {
	srand(time(0));
	int numberOfManiacs = rand() % PLAYER_COUNT;
	std::cout << "Number of maniacs: " << numberOfManiacs << std::endl;
	for (int i = 0; i < numberOfManiacs; i++) {
		int randomIndex = rand() % (PLAYER_COUNT - 1); // Exclude the human player at index 0
		botPlayer* bot = dynamic_cast<botPlayer*>(players[randomIndex].get());
		if (bot) {
			bot->makeManiac(); // Set the bot as a maniac
		}
	}
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

	Player you;
	vector<botPlayer> bots(PLAYER_COUNT - 1);
	vector<unique_ptr<Player>> players; // Using smart pointers to delete the pointer automatically at the end
	string botNames[3] = { "Amigo 1", "Mickey Mouse", "Darth Vader" };

	players.push_back(std::make_unique<Player>()); // Human player
	for (int i = 0; i < PLAYER_COUNT - 1; i++) {
		players.push_back(std::make_unique<botPlayer>());
	}

	players[0]->setName();
	for (int i = 1; i < PLAYER_COUNT; i++) {
		botPlayer* bot = dynamic_cast<botPlayer*>(players[i].get());
		if (bot) {
			bot->setNumber(i + 1);
			bot->setBotName(botNames[i - 1]);
		}
	}

	while (true) {
		std::vector<std::vector<string>> shuffledDeck = shuffleCards(Deck);

		int foldedPlayers = 0;
		int pot = 0;

		dealingCards(players, shuffledDeck);

		shuffledDeck.erase(shuffledDeck.begin(), shuffledDeck.begin() + 2 * PLAYER_COUNT); // remove used cards

		for (auto i : players[0]->getHand()) {
			std::cout << i[0] << " of " << i[1] << std::endl;
		}
		std::vector<std::vector<string>> communityCards;
		
		// Dealing the flop
		for (int i = 0; i < 3; i++)
		{
			communityCards.push_back(shuffledDeck[i]);
		}

		shuffledDeck.erase(shuffledDeck.begin(), shuffledDeck.begin() + 3);

		std::cout << std::endl;

		for (int i = 0; i < communityCards.size(); i++) {
			std::cout << "Community Card " << i + 1 << ": " << communityCards[i][0] << " of " << communityCards[i][1] << endl;
		}

		std::cout << std::endl;

		generateManiacs(players); // Randomly generate maniacs among the bots

		// First action step for the human player and bots
		flopBettingRound(players, pot, communityCards, foldedPlayers);
		std::cout << "Pot: " << pot << std::endl;
		std::cout << "." << std::endl;
		// Dealing the turn
		communityCards.push_back(shuffledDeck[0]);

		shuffledDeck.erase(shuffledDeck.begin());

		for (int i = 0; i < communityCards.size(); i++) {
			std::cout << "Community Card " << i + 1 << ": " << communityCards[i][0] << " of " << communityCards[i][1] << endl;
		}

		// Second action step for the human player and bots
		currentBet = 0;

		std::cout << "Pot: " << pot << std::endl;

		// Dealing the river
		communityCards.push_back(shuffledDeck[0]);

		shuffledDeck.erase(shuffledDeck.begin());

		for (int i = 0; i < communityCards.size(); i++) {
			std::cout << "Community Card " << i + 1 << ": " << communityCards[i][0] << " of " << communityCards[i][1] << endl;
		}

		// Third action step for the human player and bots
		currentBet = 0;
		for (int i = 0; i < PLAYER_COUNT; i++)
		{
			vector<vector<string>> cardsToPass = players[i]->getHand();
			cardsToPass.insert(cardsToPass.end(), communityCards.begin(), communityCards.end());
			if (players[i]->isFolded()) {
				continue;
			}
			checkCombinations(cardsToPass);

			// for bots: std::cout << player->getName() << " chose: " << stringFromAction(action) << std::endl;

			std::this_thread::sleep_for(std::chrono::milliseconds(1200)); // 1.2 seconds
		}
	}
	return 0;
}