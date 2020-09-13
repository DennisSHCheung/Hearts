#include "Player.h"

Player::Player(bool isPlayer0) : isPlayer0(isPlayer0)
{
}

bool Player::isHuman()
{
	return this->isPlayer0;
}

int Player::getHandSize()
{
	return this->hand.size();
}

std::set<Card>& Player::getHand()
{
	return this->hand;
}

void Player::setHand(std::set<Card> s)
{
	this->hand = s;
}

bool Player::hasTwoOfClubs()
{
	return this->hand.find(std::make_pair(2, 2)) != this->hand.end();
}

Card Player::run()
{
	
}