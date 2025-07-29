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

void botDecisionAndAct(
    int i,
    std::vector<std::unique_ptr<Player>>& players,
    std::vector<std::vector<std::string>>& communityCards,
    std::vector<int>& playerBets,
    int& currentBet,
    int& lastRaiser,
    std::vector<bool>& hasRaisedThisRound,
    std::vector<bool>& playerFolded,
    std::vector<bool>& playerAllIn,
    bool& bettingOver,
    bool& actionWasRaise,
    int& foldedPlayers
) {
    botPlayer* bot = dynamic_cast<botPlayer*>(players[i].get());
    if (!bot) { return; }

    std::vector<std::vector<std::string>> cardsToPass = players[i]->getHand();
    cardsToPass.insert(cardsToPass.end(), communityCards.begin(), communityCards.end());
    int toCall = currentBet - playerBets[i];
    std::string action = "";

    if (!bot->returnIsManiac()) {
        if (checkCombinations(players, cardsToPass, i) && !hasRaisedThisRound[i]) {
            int raiseAmt = currentBet + 100;
            int totalPut = raiseAmt - playerBets[i];
            if (players[i]->getChips() >= totalPut) {
                players[i]->setChips(players[i]->getChips() - totalPut);
                playerBets[i] = raiseAmt;
                currentBet = raiseAmt;
                hasRaisedThisRound[i] = true;
                lastRaiser = i;
                if (players[i]->getChips() == 0) { playerAllIn[i] = true; }
                action = "raise to " + std::to_string(raiseAmt);
                bettingOver = false;
                actionWasRaise = true;
            }
            else {
                if (players[i]->getChips() >= toCall) {
                    players[i]->setChips(players[i]->getChips() - toCall);
                    playerBets[i] += toCall;
                    if (players[i]->getChips() == 0) { playerAllIn[i] = true; }
                    action = "call";
                }
                else {
                    players[i]->fold();
                    playerFolded[i] = true;
                    ++foldedPlayers;
                    action = "fold";
                }
            }
        }
        else if (hasRaisedThisRound[i]) {
            if (players[i]->getChips() >= toCall) {
                players[i]->setChips(players[i]->getChips() - toCall);
                playerBets[i] += toCall;
                if (players[i]->getChips() == 0) { playerAllIn[i] = true; }
                action = "call";
            }
            else {
                players[i]->fold();
                playerFolded[i] = true;
                ++foldedPlayers;
                action = "fold";
            }
        }
        else {
            int randNum = rand() % 2;
            if (randNum == 0 && players[i]->getChips() >= toCall) {
                players[i]->setChips(players[i]->getChips() - toCall);
                playerBets[i] += toCall;
                if (players[i]->getChips() == 0) { playerAllIn[i] = true; }
                action = "call";
            }
            else {
                players[i]->fold();
                playerFolded[i] = true;
                ++foldedPlayers;
                action = "fold";
            }
        }
    }
    else { // Maniac
        if (!hasRaisedThisRound[i] && players[i]->getChips() >= toCall + 100) {
            int raiseAmt = currentBet + 100;
            int totalPut = raiseAmt - playerBets[i];
            players[i]->setChips(players[i]->getChips() - totalPut);
            playerBets[i] = raiseAmt;
            currentBet = raiseAmt;
            hasRaisedThisRound[i] = true;
            lastRaiser = i;
            if (players[i]->getChips() == 0) { playerAllIn[i] = true; }
            action = "raise to " + std::to_string(raiseAmt);
            bettingOver = false;
            actionWasRaise = true;
        }
        else if (players[i]->getChips() >= toCall) {
            players[i]->setChips(players[i]->getChips() - toCall);
            playerBets[i] += toCall;
            if (players[i]->getChips() == 0) { playerAllIn[i] = true; }
            action = "call";
        }
        else {
            players[i]->fold();
            playerFolded[i] = true;
            ++foldedPlayers;
            action = "fold";
        }
    }
    std::cout << players[i]->getName() << " chose: " << action << std::endl;
}


void BettingRound(std::vector<std::unique_ptr<Player>>& players, int& pot,
    std::vector<std::vector<std::string>>& communityCards, int& foldedPlayers) {

    int currentBet = 0;
    int numPlayers = PLAYER_COUNT;
    std::vector<int> playerBets(numPlayers, 0);
    std::vector<bool> playerFolded(numPlayers, false);
    std::vector<bool> playerAllIn(numPlayers, false);
    std::vector<bool> hasRaisedThisRound(numPlayers, false);

    int lastRaiser = -1;
    bool bettingOver = false;
    bool firstCycle = true;

    // BETTING ROUND LOOP
    while (!bettingOver) {
        bettingOver = true;
        for (int i = 0; i < numPlayers; ++i) {
            if (playerFolded[i] || playerAllIn[i]) {
                continue;
            }

            // HUMAN PLAYER
            if (i == 0) {
                if (playerBets[0] < currentBet) {
                    std::cout << "Current bet: " << currentBet << ", your bet: " << playerBets[0]
                        << ", your chips: " << players[0]->getChips() << std::endl;
                    std::cout << "Do you want to (1) Fold, (2) Call (" << (currentBet - playerBets[0]) << "), (3) Raise? ";
                    int choice;
                    while (true) {
                        std::cin >> choice;
                        if (choice < 1 || choice > 3) {
                            std::cout << "Invalid choice. Enter 1, 2, or 3: ";
                            continue;
                        }
                        if (choice == 3 && hasRaisedThisRound[0]) {
                            std::cout << "You have already raised this round. Choose 1 or 2: ";
                            continue;
                        }
                        break;
                    }
                    if (choice == 1) {
                        players[0]->fold();
                        playerFolded[0] = true;
                        ++foldedPlayers;
                        std::cout << players[0]->getName() << " chose: fold" << std::endl;
                        continue;
                    }
                    else if (choice == 2) {
                        int toCall = currentBet - playerBets[0];
                        if (toCall > players[0]->getChips()) { toCall = players[0]->getChips(); }
                        players[0]->setChips(players[0]->getChips() - toCall);
                        playerBets[0] += toCall;
                        if (players[0]->getChips() == 0) { playerAllIn[0] = true; }
                        std::cout << players[0]->getName() << " chose: call" << std::endl;
                    }
                    else {
                        std::cout << "Enter raise total (must be more than " << currentBet
                            << ", up to " << (playerBets[0] + players[0]->getChips()) << "): ";
                        int raiseTo;
                        while (true) {
                            std::cin >> raiseTo;
                            if (raiseTo <= currentBet || raiseTo > playerBets[0] + players[0]->getChips()) {
                                std::cout << "Invalid raise amount. Try again: ";
                                continue;
                            }
                            break;
                        }
                        int toAdd = raiseTo - playerBets[0];
                        players[0]->setChips(players[0]->getChips() - toAdd);
                        playerBets[0] = raiseTo;
                        currentBet = raiseTo;
                        hasRaisedThisRound[0] = true;
                        lastRaiser = 0;
                        if (players[0]->getChips() == 0) { playerAllIn[0] = true; }
                        std::cout << players[0]->getName() << " chose: raise to " << raiseTo << std::endl;
                        bettingOver = false;
                        break;
                    }
                }
                else if (firstCycle && currentBet == 0 && !hasRaisedThisRound[0]) {
                    std::cout << "Current bet: " << currentBet << ", your bet: " << playerBets[0]
                        << ", your chips: " << players[0]->getChips() << std::endl;
                    std::cout << "Do you want to (1) Check, (2) Bet? ";
                    int choice;
                    while (true) {
                        std::cin >> choice;
                        if (choice < 1 || choice > 2) {
                            std::cout << "Invalid choice. Enter 1 or 2: ";
                            continue;
                        }
                        break;
                    }
                    if (choice == 1) {
                        std::cout << players[0]->getName() << " chose: check" << std::endl;
                    }
                    else {
                        std::cout << "Enter bet (up to " << players[0]->getChips() << "): ";
                        int bet;
                        while (true) {
                            std::cin >> bet;
                            if (bet <= 0 || bet > players[0]->getChips()) {
                                std::cout << "Invalid bet. Try again: ";
                                continue;
                            }
                            break;
                        }
                        players[0]->setChips(players[0]->getChips() - bet);
                        playerBets[0] = bet;
                        currentBet = bet;
                        hasRaisedThisRound[0] = true;
                        lastRaiser = 0;
                        if (players[0]->getChips() == 0) { playerAllIn[0] = true; }
                        std::cout << players[0]->getName() << " chose: bet " << bet << std::endl;
                        bettingOver = false;
                        break;
                    }
                }
            }
            else {
                bool actionWasRaise = false;
                botDecisionAndAct(
                    i, players, communityCards, playerBets, currentBet, lastRaiser, hasRaisedThisRound,
                    playerFolded, playerAllIn, bettingOver, actionWasRaise, foldedPlayers
                );
                // Now, EVERY action is printed within botDecisionAndAct.
                if (actionWasRaise) {
                    bettingOver = false;
                    break;
                }
            }
        }
        // Betting round ends if all (except lastRaiser, folders, all-in) have matched currentBet
        bettingOver = true;
        for (int i = 0; i < numPlayers; ++i) {
            if (i == lastRaiser || playerFolded[i] || playerAllIn[i]) { continue; }
            if (playerBets[i] != currentBet) { bettingOver = false; break; }
        }
        firstCycle = false;
    }

    // Gather all bets into pot
    for (int i = 0; i < numPlayers; ++i) {
        pot += playerBets[i];
    }
}
