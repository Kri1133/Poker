#pragma once  
#include <string>  
#include <vector>  

enum class Action {
	bet,
	fold,
	call,
	raise,
};

class Player
{
protected: // Change private members to protected to allow access in derived classes  
	int playerNumber;
	int chips = 10000; // Each player starts with 10,000 chips
	bool isBusted = false;
	bool folded = false; // Flag to indicate if the player has folded
	bool bot = false; // Flag to indicate if the player is a bot
	bool isAllIn = false;
	bool isWinner = false;
	std::string name;
	std::string highestCombination = "";
	std::vector<std::vector<std::string>> hand; // Each player gets 2 cards  
	std::string action;

public:
	virtual ~Player() {}
	void setNumber(const int i);
	int getNumber();
	void setName();
	std::string getName();
	void setHighestCombination(const std::string& combination) { highestCombination = combination; }
	std::string getHighestCombination() const { return highestCombination; }
	void resetHighestCombination() { highestCombination = ""; }
	bool isFolded();
	void setIsWinner() { isWinner = true; }
	bool returnIsWinner() const { return isWinner; }
	void resetIsWinner() { isWinner = false; }
	void setIsBusted() { isBusted = true; }
	bool returnIsBusted() const { return isBusted; }
	void resetIsFolded() { folded = false; }
	int getChips() const { return chips; }
	void setChips(int amount) { chips = amount; }
	bool getIsAllIn() const { return isAllIn; }
	void setIsAllIn() { isAllIn = true; }
	void resetIsAllIn() { isAllIn = false; }
	virtual bool isBot() const { return false; }
	void addCardToHand(std::vector<std::string> card);
	void resetHand() { hand.clear(); }
	std::vector<std::vector<std::string>> getHand();
	std::vector<std::vector<std::string>> getBestFiveCardHand(std::vector<std::vector<std::string>>& communityCards);
	virtual Action chooseAction(int& bet); // virtual function to allow derived classes to implement their own action logic (override it))

	void fold();
	void bet(int& bet);
	void call(int& bet);
	virtual void raise(int& bet);
};

class botPlayer : public Player
{
	// Bot player class is used to implement AI for the game   
	bool goodCombination = false;
	bool isManiac = false; // Flag to indicate if the bot is a risky player
	bool hasRaisedFlag = false; // Flag to indicate if the player has raised in the current round
public:
	void setBotName(const std::string& name);
	void makeManiac();
	void raise(int& bet) override;
	bool isBot() const override { return true; };
	bool returnIsManiac();
	bool hasRaised() const { return hasRaisedFlag; }
	void resetHasRaised() { hasRaisedFlag = false; }
	void setHasRaised() { hasRaisedFlag = true; }
	void resetIsManiac() { isManiac = false; }
	};