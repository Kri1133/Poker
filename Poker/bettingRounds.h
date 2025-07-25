#pragma once
#include <vector>
#include <string>
#include <memory>
#include "player.h"
#include "action_utils.h"
#include "hand_combinations.h"
#include "combination_check.h"

void flopBettingRound(std::vector<std::unique_ptr<Player>>& players, int& pot,
	std::vector<std::vector<std::string>>& communityCards, int& foldedPlayers);

void turnBettingRound(std::vector<std::unique_ptr<Player>>& players, int& pot,
	std::vector<std::vector<std::string>>& communityCards, int& foldedPlayers);

void riverBettingRound(std::vector<std::unique_ptr<Player>>& players, int& pot,
	std::vector<std::vector<std::string>>& communityCards, int& foldedPlayers);