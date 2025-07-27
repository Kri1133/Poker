#pragma once
#include <string>
#include <algorithm>
#include <stdexcept>
#include "player.h"

// Converts string (case-insensitive) to Action enum
inline Action actionFromString(const std::string& input) {
    std::string s = input;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    if (s == "fold") return Action::fold;
    if (s == "call") return Action::call;
    if (s == "bet")  return Action::bet;
    if (s == "raise") return Action::raise;
    throw std::invalid_argument("Invalid action: " + input);
}

// Converts Action enum to user-friendly string
inline std::string stringFromAction(Action action) {
    switch (action) {
    case Action::fold:  return "Fold";
    case Action::call:  return "Call";
    case Action::bet:   return "Bet";
    case Action::raise: return "Raise";
    default:            return "Unknown";
    }
}
