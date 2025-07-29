#pragma once
#include <vector>
#include <string>
#include <memory>
#include "player.h"
#include "actionUtils.h"
#include "handCombinations.h"
#include "combinationCheck.h"

void BettingRound(std::vector<std::unique_ptr<Player>>& players, int& pot,
	std::vector<std::vector<std::string>>& communityCards, int& foldedPlayers);
