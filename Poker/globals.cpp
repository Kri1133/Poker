#include "globals.h"

int currentBet = 0;
int prevBet = 0;
int PLAYER_COUNT = 4;
bool bettingDone = false;
int indexOfPlayerWhoRaised = -1;
std::map<int, std::string> rankMap = { {2, "2"}, {3, "3"}, {4, "4"}, {5, "5"}, {6, "6"}, {7, "7"}, {8, "8"},{9, "9"}, {10, "10"}, {11, "Jack"}, {12, "Queen"}, {13, "King"}, {14, "Ace"} };
std::map<std::string, int> rankToValue = {
	{"2", 2},
	{"3", 3},
	{"4", 4},
	{"5", 5},
	{"6", 6},
	{"7", 7},
	{"8", 8},
	{"9", 9},
	{"10", 10},
	{"Jack", 11},
	{"Queen", 12},
	{"King", 13},
	{"Ace", 14}
};
std::map<std::string, int> combinationsRank = {
	{ "High Card", 1},
	{ "One Pair", 2},
	{ "Two Pair", 3},
	{ "Three of a Kind", 4},
	{ "Straight", 5},
	{ "Flush", 6},
	{ "Full House", 7},
	{ "Four of a Kind", 8},
	{ "Straight Flush", 9},
	{ "Royal Flush", 10}
};