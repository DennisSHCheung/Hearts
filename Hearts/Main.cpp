#include "Common.h"
#include "Player.h"

sf::Vector2f getMouseCoord(sf::RenderWindow& window)
{
	sf::Vector2i mouse_position = sf::Mouse::getPosition(window); // Mouse position related to the window
	sf::Vector2f translated_position = window.mapPixelToCoords(mouse_position); // Translate mouse position into window coordinates
	return translated_position;
}

std::string getExeLocation()
{
	char result[MAX_PATH];
	std::string path = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
	path = path.substr(0, path.find_last_of("\\"));
	path += "\\assets\\";
	return path;
}

Deck initDeck()
{
	Deck deck;
	deck.reserve(52);
	for (int i = 0; i < 4; i++)
		for (int j = 1; j <= 13; j++)
			deck.push_back(std::make_pair(i, j));
	return deck;
}

CardSprites initSprites(sf::Texture& t)
{
	std::vector<sf::Sprite> sp(53);
	for (int i = 0; i < 4; i++)
	{
		int suit = 2;
		if (i == 1) suit = 3;
		else if (i == 2) suit = 1;
		else if (i == 3) suit = 0;
		int index = suit * 13;
		for (int j = 1; j <= 13; j++)
		{
			sf::Sprite s;
			s.setTexture(t);
			s.setTextureRect(sf::IntRect(79 * (j - 1), 123 * i, 79, 123));
			sp[index++] = s;
		}
	}
	
	// non-revealed 
	sf::Sprite s;
	s.setTexture(t);
	s.setTextureRect(sf::IntRect(79 * 2, 123 * 4, 79, 123));
	sp[52] = s;

	return sp;
}

void shuffleDeck(Deck& deck)
{
	std::random_shuffle(deck.begin(), deck.end());
}

void setCardsPosition(CardSprites& cardSprites,
	PlayerList& playersList, int playerIndex)
{
	int cardCount = 0, defaultX, defaultY;
	if (playerIndex == 0 || playerIndex == 2)
	{
		defaultX = 350;
		if (playerIndex == 0) defaultY = 660;
		else if (playerIndex == 2) defaultY = 20;
		for (auto& c : playersList[playerIndex]->getHand())
		{
			cardSprites[c.first * 13 + c.second - 1].setPosition(
				sf::Vector2f(defaultX + cardCount * 30, defaultY));
			cardCount++;
		}
	}
	else
	{
		defaultY = 150;
		if (playerIndex == 1) defaultX = 50;
		else if (playerIndex == 3) defaultX = 1050;
		for (auto& c : playersList[playerIndex]->getHand())
		{
			cardSprites[c.first * 13 + c.second - 1].setPosition(
				sf::Vector2f(defaultX, defaultY + cardCount * 30));
			cardCount++;
		}
	}	
}

void display(sf::RenderWindow& window, CardSprites& cardSprites)
{
	for (int i = 0; i < 52; i++)
		window.draw(cardSprites[i]);

	window.display();
}

void highlightCard(PlayerList& playersList, CardSprites& cardSprites, sf::RenderWindow& window)
{
	int index = -1;
	sf::Vector2f oldPosition(0.0, 0.0);
	for (auto& c : playersList[0]->getHand())
		if (cardSprites[c.first * 13 + c.second - 1].getGlobalBounds().contains(getMouseCoord(window)))
			index = c.first * 13 + c.second - 1;

	if (index != -1)
	{
		oldPosition = cardSprites[index].getPosition();
		sf::Vector2f newPosition = { oldPosition.x, oldPosition.y - 50 };
		cardSprites[index].setPosition(newPosition);
	}
}

int main()
{
	std::srand(unsigned(std::time(0)));

	sf::RenderWindow window(sf::VideoMode(1200, 800), "Hearts");
	window.setFramerateLimit(30);

	sf::Texture cardTexture;
	cardTexture.loadFromFile(getExeLocation() + "playingCards.png");
	
	CardSprites cardSprites = initSprites(cardTexture);

	Deck deck = initDeck();
	shuffleDeck(deck);

	// Player 0 is a human player
	PlayerList playersList;
	for (int i = 0; i < 4; i++)
	{
		if (i == 0) playersList.push_back(std::make_unique<Player>(true));
		else playersList.push_back(std::make_unique<Player>(false));
		playersList[i]->setHand(std::set<Card>(deck.begin() + i * 13, deck.begin() + (i + 1) * 13));
	}

	unsigned int trickCount = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);
		
		for (int i = 0; i < 4; i++)
			setCardsPosition(cardSprites, playersList, i);

		highlightCard(playersList, cardSprites, window);

		display(window, cardSprites);
	}

	return 0;
}