#include <vector>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include "player.h"
#include "actionUtils.h"
#include "handCombinations.h"
#include "globals.h"
#include "combinationCheck.h"

// Helper for hand strength vector
auto getHandStrengthVector = [](const std::vector<std::vector<std::string>>& hand) {
    std::map<std::string, int> counts;
    for (const auto& card : hand)
        counts[card[0]]++;
    std::vector<std::pair<int, int>> grouped;
    for (const auto& [rank, cnt] : counts)
        grouped.push_back({ cnt, rankToValue.at(rank) });
    std::sort(grouped.rbegin(), grouped.rend());
    std::vector<int> result;
    for (const auto& [cnt, val] : grouped)
        for (int i = 0; i < cnt; ++i)
            result.push_back(val);
    return result;
    };

void getWinner(std::vector<std::unique_ptr<Player>>& players, int& pot,
    std::vector<std::vector<std::string>>& communityCards, int& /*foldedPlayers*/) {

    int bestRank = 0;
    std::vector<int> indecesOfBestPlayers;
    std::vector<std::vector<int>> allStrengths(players.size());

    // Evaluate ONLY non-folded, non-busted players
    for (int i = 0; i < PLAYER_COUNT; i++) {
        if (players[i]->isFolded() || players[i]->returnIsBusted()) continue;

        // Always get the best 5-card hand at showdown
        auto bestHand = players[i]->getBestFiveCardHand(communityCards);
        std::string combo = getCombinationName(bestHand);
        int rank = combinationsRank[combo];
        auto strength = getHandStrengthVector(bestHand);
        allStrengths[i] = strength;

        if (rank > bestRank) {
            bestRank = rank;
            indecesOfBestPlayers.clear();
            indecesOfBestPlayers.push_back(i);
        }
        else if (rank == bestRank) {
            indecesOfBestPlayers.push_back(i);
        }
    }

    if (indecesOfBestPlayers.empty()) {
        std::cout << "No winner: all players folded or busted." << std::endl;
        return;
    }

    // Break ties by hand strength vector
    std::vector<int> bestPlayers;
    std::vector<int> bestStrength;
    for (int idx : indecesOfBestPlayers) {
        if (bestPlayers.empty() || allStrengths[idx] > bestStrength) {
            bestPlayers.clear();
            bestPlayers.push_back(idx);
            bestStrength = allStrengths[idx];
        }
        else if (allStrengths[idx] == bestStrength) {
            bestPlayers.push_back(idx);
        }
    }

    if (bestPlayers.size() > 1) {
        std::cout << "It's a tie! Pot is split between: ";
        for (int idx : bestPlayers) {
            std::cout << players[idx]->getName() << " ";
            players[idx]->setIsWinner();
            players[idx]->setChips(players[idx]->getChips() + pot / bestPlayers.size());
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "The winner is " << players[bestPlayers[0]]->getName() << std::endl;
        players[bestPlayers[0]]->setIsWinner();
        players[bestPlayers[0]]->setChips(players[bestPlayers[0]]->getChips() + pot);
    }
}
