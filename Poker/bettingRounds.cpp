#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include "player.h"
#include "action_utils.h"
#include "hand_combinations.h"
#include "combination_check.h"
#include "bettingRounds.h"
#include "globals.h"


void flopBettingRound(std::vector<std::unique_ptr<Player>>& players, int& pot,
	std::vector<std::vector<std::string>>& communityCards, int& foldedPlayers) {
	int currentBet = 0;
	std::vector<int> playerBets(PLAYER_COUNT, 0); // Tracks each player's bet in this round
	std::vector<bool> hasActed(PLAYER_COUNT, false);

	int numActive = PLAYER_COUNT - foldedPlayers;

	bool bettingDone = false;
	while (!bettingDone) {
		bettingDone = true; // Assume done unless a raise occurs
		for (int i = 0; i < PLAYER_COUNT; i++) {
			if (players[i]->isFolded()) continue;
			if (playerBets[i] == currentBet && hasActed[i]) continue; // Already matched bet and acted after last raise

			std::vector<std::vector<std::string>> cardsToPass = players[i]->getHand();
			cardsToPass.insert(cardsToPass.end(), communityCards.begin(), communityCards.end());
			std::string botAction = "";
			botPlayer* bot = dynamic_cast<botPlayer*>(players[i].get());
			if (bot && !bot->returnIsManiac()) {
				if (checkCombinations(cardsToPass) && !bot->hasRaised()) {
					players[i]->raise(pot, currentBet);
					bettingDone = false; // A raise means we need to continue betting
					bot->setHasRaised(); // Mark that this bot has raised
					botAction = "raise";
				}
				else {
					int randNum = rand() % 2; // Randomly choose between call and fold
					if (bot->hasRaised()) {
						players[i]->call(pot, currentBet);
						botAction = "call";
					}
					else {
						if (randNum == 0 || isGoodStartingHand(players[i]->getHand())) {
							players[i]->call(pot, currentBet);
							botAction = "call";
						}
						else {
							players[i]->fold();
							botAction = "fold";

						}
						foldedPlayers++;
					}
				}
				std::cout << players[i]->getName() << " chose: " << botAction << std::endl;
			}
			else if (bot && bot->returnIsManiac())
			{
				if (!bot->hasRaised()) {
					bot->raise(pot, currentBet);
					botAction = "raise";
					std::cout << players[i]->getName() << " chose: " << botAction << std::endl;
					bettingDone = false; // A raise means we need to continue betting
					bot->setHasRaised(); // Mark that this bot has raised
				}
			}
			else {
				Action action = players[i]->chooseAction(pot, currentBet);
				switch (action)
				{
				case Action::bet:
					break;
				case Action::fold:
					players[i]->fold();
					foldedPlayers++;
					break;
				case Action::call:
					break;
				case Action::raise:
					break;
				default:
					break;
				}
			}
			hasActed[i] = true;
			std::cout << players[i]->getName() << " chose: " << botAction << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(700)); // Shorter pause
	}
	// Check if all non-folded players have matched the currentBet
	for (int i = 0; i < PLAYER_COUNT; i++) {
		if (players[i]->isFolded()) continue;
		if (playerBets[i] != currentBet) bettingDone = false;
	}
}

void turnBettingRound(std::vector<std::unique_ptr<Player>>& players, int& pot,
	std::vector<std::vector<std::string>>& communityCards, int& foldedPlayers)
{
	for (int i = 0; i < PLAYER_COUNT; i++)
	{
		std::vector<std::vector<std::string>> cardsToPass = players[i]->getHand();
		cardsToPass.insert(cardsToPass.end(), communityCards.begin(), communityCards.end());
		if (players[i]->isFolded()) {
			continue;
		}
		auto combos = getAllFiveCardCombinations(cardsToPass);
		if (players[i]->isBot()) {
			bool goodCombo = false;
			for (const auto& hand : combos) {
				if (checkCombinations(cardsToPass)) {
					goodCombo = true;
					break;
				}
			}
			if (goodCombo) {
				players[i]->raise(pot, currentBet);
				std::cout << players[i]->getName() << " chose: Raise!" << std::endl;
			}
			else if (isGoodStartingHand(players[i]->getHand())) {
				players[i]->call(pot, currentBet);
				std::cout << players[i]->getName() << " chose: Call!" << std::endl;
			}
			else {
				players[i]->fold();
				std::cout << players[i]->getName() << " chose: Fold!" << std::endl;
				foldedPlayers++;
			}
			// call doesn't work for bots or human player
		}
		else {
			players[i]->chooseAction(pot, currentBet);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // 3 seconds
	}
}

void riverBettingRound(std::vector<std::unique_ptr<Player>>& players, int& pot,
	std::vector<std::vector<std::string>>& communityCards, int& foldedPlayers) {

}