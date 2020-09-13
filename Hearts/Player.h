#pragma once

#include "Common.h"

class Player
{
private:
	bool isPlayer0 = false;
	std::set<Card> hand;
public:
	Player(bool isPlayer0);
	bool isHuman();
	int getHandSize();
	std::set<Card>& getHand();
	bool hasTwoOfClubs();

	void setHand(std::set<Card> s);

	Card run();
};

typedef std::vector<std::unique_ptr<Player>> PlayerList;
