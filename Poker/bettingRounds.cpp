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

void BettingRound(std::vector<std::unique_ptr<Player>>& players, int& pot,
	std::vector<std::vector<std::string>>& communityCards, int& foldedPlayers) {
	int currentBet = 0;

	while (!bettingDone) {
		bettingDone = true; // Assume done unless a raise occurs
		for (int i = 0; i < PLAYER_COUNT; i++) {
			if (i == indexOfPlayerWhoRaised) {
				break;
			}
			if (players[i]->returnIsBusted() || players[i]->isFolded() || players[i]->getIsAllIn()) {
				std::cout << "!!!!!!!!!!!!!!!" << std::endl;
				continue;
			}

			std::vector<std::vector<std::string>> cardsToPass = players[i]->getHand();
			cardsToPass.insert(cardsToPass.end(), communityCards.begin(), communityCards.end());
			std::string botAction = "";
			botPlayer* bot = dynamic_cast<botPlayer*>(players[i].get());
			if (bot && !bot->returnIsManiac()) {
				if (checkCombinations(players, cardsToPass, i) && !bot->hasRaised()) {
					bot->raise();
					bot->setHasRaised();
					botAction = "raise";
					std::cout << botAction << " by " << bot->getName() << std::endl;
					indexOfPlayerWhoRaised = i;
				}
				else if (bot->hasRaised()) {
					players[i]->call();
					std::cout << "call by " << bot->getName() << std::endl;
					botAction = "call";
				}
				else {
					int randNum = rand() % 2; // Randomly choose between call and fold
					if (randNum == 0) {
						bot->call();
						botAction = "call";
						std::cout << "call by " << bot->getName() << std::endl;
					}
					else {
						bot->fold();
						botAction = "fold";
						std::cout << botAction << " by " << bot->getName() << std::endl;
					}
				}
			}
			else if (bot && bot->returnIsManiac())
			{
				if (!(bot->hasRaised())) {
					bot->raise();
					bot->setHasRaised();
					botAction = "raise";
					std::cout << botAction << " by " << bot->getName() << std::endl;
					indexOfPlayerWhoRaised = i;
				}
				else {
					bot->call();
					botAction = "call";
					std::cout << botAction << " by " << bot->getName() << std::endl;
				}
			}
			else {
				Action action = players[i]->chooseAction(currentBet);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Shorter pause
	}
	for (int i = 0; i < PLAYER_COUNT; i++) {
		if (players[i]->isFolded()) { continue; }
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
	for (int i = 0; i < PLAYER_COUNT; ++i) {
		if (!(players[i]->isFolded()) && !(players[i]->returnIsBusted())) {
			pot += players[i]->getAmountAlreadyPut();
		}
		players[i]->resetAmountAlreadyPut();
	}

}

// Fix raise() and betting every round

