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
	bool folded = false; // Flag to indicate if the player has folded
	bool bot = false; // Flag to indicate if the player is a bot
	std::string name;
	std::vector<std::vector<std::string>> hand; // Each player gets 2 cards  
	std::string action;

public:
	virtual ~Player() {}
	void setNumber(const int i);
	int getNumber();
	void setName();
	std::string getName();
	bool isFolded();
	virtual bool isBot() const { return false; }
	void addCardToHand(std::vector<std::string> card);
	std::vector<std::vector<std::string>> getHand();
	virtual Action chooseAction(int& pot, int& bet); // virtual function to allow derived classes to implement their own action logic (override it))

	void fold();
	void bet(int& pot, int& bet);
	void call(int& pot, int& bet);
	virtual void raise(int& pot, int& bet);
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
	void raise(int& pot, int& bet) override;
	bool isBot() const override { return true; };
	bool returnIsManiac();
	bool hasRaised() const { return hasRaisedFlag; }
	void setHasRaised() { hasRaisedFlag = true; }
	void undoSetHasRaised() { hasRaisedFlag = false; }
};