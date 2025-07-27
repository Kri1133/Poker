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
    bettingDone = false; // Always reset before a round
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
                    int prevBet = currentBet;
                    players[i]->raise(currentBet);
                    int betDiff = currentBet - prevBet;
                    players[i]->setChips(players[i]->getChips() - betDiff);
                    playerBets[i] += betDiff;
                    bot->setHasRaised(); // Mark that this bot has raised
                    botAction = "raise";
                    indexOfPlayerWhoRaised = i;
                }
                else if (bot->hasRaised()) {
                    int toCall = currentBet - playerBets[i];
                    if (toCall > 0) {
                        players[i]->setChips(players[i]->getChips() - toCall);
                        playerBets[i] += toCall;
                    }
                    players[i]->call(currentBet);
                    botAction = "call";
                }
                else {
                    int randNum = rand() % 2; // Randomly choose between call and fold
                    if (randNum == 0) {
                        int toCall = currentBet - playerBets[i];
                        if (toCall > 0) {
                            players[i]->setChips(players[i]->getChips() - toCall);
                            playerBets[i] += toCall;
                        }
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
                    int prevBet = currentBet;
                    bot->raise(currentBet);
                    int betDiff = currentBet - prevBet;
                    players[i]->setChips(players[i]->getChips() - betDiff);
                    playerBets[i] += betDiff;
                    botAction = "raise";
                    indexOfPlayerWhoRaised = i;
                    bot->setHasRaised();
                }
                else {
                    int toCall = currentBet - playerBets[i];
                    if (toCall > 0) {
                        players[i]->setChips(players[i]->getChips() - toCall);
                        playerBets[i] += toCall;
                    }
                    players[i]->call(currentBet);
                    botAction = "call";
                }
            }
            else {
                // Human player
                Action action = players[i]->chooseAction(currentBet);
                switch (action)
                {
                case Action::bet: {
                    int betAmt = currentBet;
                    players[i]->setChips(players[i]->getChips() - betAmt);
                    playerBets[i] += betAmt;
                    botAction = "bet";
                    break;
                }
                case Action::fold:
                    botAction = "fold";
                    foldedPlayers++;
                    break;
                case Action::call: {
                    int toCall = currentBet - playerBets[i];
                    if (toCall > 0) {
                        players[i]->setChips(players[i]->getChips() - toCall);
                        playerBets[i] += toCall;
                    }
                    botAction = "call";
                    break;
                }
                case Action::raise: {
                    int prevBet = playerBets[i];
                    int raiseAmt = currentBet - prevBet;
                    players[i]->setChips(players[i]->getChips() - raiseAmt);
                    playerBets[i] += raiseAmt;
                    botAction = "raise";
                    indexOfPlayerWhoRaised = i;
                    break;
                }
                default:
                    break;
                }
            }
            std::cout << players[i]->getName() << " chose: " << botAction << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Shorter pause
    }

    // Move all player bets into the pot and reset playerBets
    for (int i = 0; i < PLAYER_COUNT; i++) {
        if (players[i]->isFolded() || players[i]->returnIsBusted()) continue;
        pot += playerBets[i];
        playerBets[i] = 0;
        botPlayer* bot = dynamic_cast<botPlayer*>(players[i].get());
        if (bot) {
            bot->resetHasRaised();
        }
    }
}
