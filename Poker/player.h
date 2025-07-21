#pragma once  
#include <string>  
#include <vector>  

enum class Action {
	Fold,
	Call,
	Raise,
	AllIn
};

class Player
{
protected: // Change private members to protected to allow access in derived classes  
	int playerNumber;
	bool goodCombination = false;
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
};

class botPlayer : public Player
{
	// Bot player class is used to implement AI for the game  
public:
	void setBotName(const std::string& name);
	Action chooseAction() override;
};