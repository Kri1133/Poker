#pragma once
#include <vector>
#include <string>

extern int currentBet; // Current bet amount in the game
extern int PLAYER_COUNT; // Number of players in the game
extern bool checkCombinations(std::vector<std::vector<std::string>> hand);
extern bool bettingDone;
extern int indexOfPlayerWhoRaised;