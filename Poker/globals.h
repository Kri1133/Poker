#pragma once
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <map>
#include "player.h"

extern int currentBet; // Current bet amount in the game
extern int prevBet; // Previous bet amount in the game
extern int PLAYER_COUNT; // Number of players in the game
extern bool checkCombinations(std::vector<std::unique_ptr<Player>>& players, 
	std::vector<std::vector<std::string>> hand, int i);
extern bool bettingDone;
extern int indexOfPlayerWhoRaised;
extern std::map<int, std::string> rankMap;
extern std::map<std::string, int> rankToValue;
extern std::map<std::string, int> combinationsRank;