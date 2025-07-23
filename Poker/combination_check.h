#pragma once
#include <iostream>
#include <vector>
#include <string>

bool isRoyalFlush(std::vector<std::vector<std::string>>& hand);
bool isFourOfaKind(std::vector<std::vector<std::string>>& hand);
bool isFullHouse(std::vector<std::vector<std::string>>& hand);
bool isFlush(std::vector<std::vector<std::string>>& hand);
bool isStraight(std::vector<std::vector<std::string>> hand);
bool isThreeOfaKind(std::vector<std::vector<std::string>>& hand);
int isPair(std::vector<std::vector<std::string>>& hand);
int getValueFromRank(const std::string& rank);
bool isGoodStartingHand(const std::vector<std::vector<std::string>>& hand);