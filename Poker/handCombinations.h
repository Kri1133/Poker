#pragma once
#include <vector>
#include <string>

// Returns all unique 5-card combinations from the input set
// The point is not to rewrite the combination functions
std::vector<std::vector<std::vector<std::string>>> getAllFiveCardCombinations(
    const std::vector<std::vector<std::string>>& cards);
