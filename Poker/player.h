#pragma once  
#include <string>  
#include <vector>  

enum class Action {
	Bet,
	Fold,
	Call,
	Raise,
	AllIn
};

class Player
{
protected: // Change private members to protected to allow access in derived classes  
	int playerNumber;
	int currentBet = 0;
	int pot = 0;
	int chips = 10000; // Each player starts with 10,000 chips
	bool folded = false; // Flag to indicate if the player has folded
	std::string name;
	std::vector<std::vector<std::string>> hand; // Each player gets 2 cards  
	std::string action;

public:
	virtual ~Player() {}
	void setNumber(const int i);
	int getNumber();
	void setName();
	std::string getName();
	void addCardToHand(std::vector<std::string> card);
	std::vector<std::vector<std::string>> getHand();
	virtual Action chooseAction(); // virtual function to allow derived classes to implement their own action logic (override it))

	void bet();
	void call();
	void fold();
	void raise();
};

class botPlayer : public Player
{
	// Bot player class is used to implement AI for the game 
	bool goodCombination = false;
public:
	void setBotName(const std::string& name);
	Action chooseAction() override;
};