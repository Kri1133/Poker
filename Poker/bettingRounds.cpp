#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include "player.h"
#include "actionUtils.h"
#include "handCombinations.h"
#include "combinationCheck.h"
#include "bettingRounds.h"
#include "globals.h"

#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include "player.h"
#include "actionUtils.h"
#include "handCombinations.h"
#include "combinationCheck.h"
#include "bettingRounds.h"
#include "globals.h"


void flopBettingRound(std::vector<std::unique_ptr<Player>>& players, int& pot,
	std::vector<std::vector<std::string>>& communityCards, int& foldedPlayers) {
	int currentBet = 0;
	int numActive = PLAYER_COUNT - foldedPlayers;
	std::vector<int> playerBets(PLAYER_COUNT, 0); // Tracks each player's bet in this round

	while (!bettingDone) {
		bettingDone = true; // Assume done unless a raise occurs
		for (int i = 0; i < PLAYER_COUNT; i++) {
			if (i == indexOfPlayerWhoRaised) { break; }
			if (players[i]->returnIsBusted() || players[i]->isFolded()) { continue; }

			std::vector<std::vector<std::string>> cardsToPass = players[i]->getHand();
			cardsToPass.insert(cardsToPass.end(), communityCards.begin(), communityCards.end());
			std::string botAction = "";
			botPlayer* bot = dynamic_cast<botPlayer*>(players[i].get());
			if (bot && !bot->returnIsManiac()) {
				if (checkCombinations(players, cardsToPass, i) && !bot->hasRaised()) {
					players[i]->raise(currentBet);
					bot->setHasRaised(); // Mark that this bot has raised
					botAction = "raise";
					indexOfPlayerWhoRaised = i;
				}
				else if (bot->hasRaised()) {
					players[i]->call(currentBet);
					botAction = "call";
				}
				else {
					int randNum = rand() % 2; // Randomly choose between call and fold
					if (randNum == 0) {
						players[i]->call(currentBet);
						botAction = "call";
					}
					else {
						players[i]->fold();
						botAction = "fold";
						foldedPlayers++;
					}
				}
			}
			else if (bot && bot->returnIsManiac())
			{
				if (!bot->hasRaised()) {
					bot->raise(currentBet);
					botAction = "raise";
					indexOfPlayerWhoRaised = i;
					bot->setHasRaised();
				}
				else {
					players[i]->call(currentBet);
					botAction = "call";
				}
			}
			else {
				Action action = players[i]->chooseAction(currentBet);
				switch (action)
				{
				case Action::bet:
					botAction = "bet";
					break;
				case Action::fold:
					botAction = "fold";
					foldedPlayers++;
					break;
				case Action::call:
					botAction = "call";
					break;
				case Action::raise:
					botAction = "raise";
					indexOfPlayerWhoRaised = i;
					break;
				default:
					break;
				}
			}
			std::cout << players[i]->getName() << " chose: " << botAction << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Shorter pause
	}
	for (int i = 0; i < PLAYER_COUNT; i++) {
		if (players[i]->isFolded()) continue;
		if (playerBets[i] != currentBet) bettingDone = false;
		botPlayer* bot = dynamic_cast<botPlayer*>(players[i].get());
		if (bot) {
			bot->resetHasRaised();
		}
		if (players[i]->getIsAllIn()) {
			pot += players[i]->getChips();
			continue;
		}
		pot += currentBet;
	}
	bettingDone = false;
}

