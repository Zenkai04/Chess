#pragma once

class GraphicManager;

#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Piece.h"
#include "CaseColor.h"

const sf::Vector2f caseSize = sf::Vector2f(100, 100);
const sf::Vector2f boardShiftPos = sf::Vector2f(50, 50);

class GraphicManager
{
public:
	GraphicManager(sf::RenderWindow* window, sf::Vector2i windowSize, Board* board);
	~GraphicManager();

	void updateRendering();	
	void renderMatPlayer(Player* matPlayer);

	sf::RenderWindow* getWindow();

private:
	void loadFonts();
	void renderBoard();
	void initialiseBoard();
	sf::Color getGraphicColor(CaseColor caseColor);

	void renderPieces();
	void initialisePieces();
	void renderReachablePositions();
	void renderUi();

	sf::Vector2i windowSize;
	sf::RenderWindow* window;

	Board* board;

	sf::RectangleShape*** cases;
	bool piecesInitilized = false;

	sf::Sprite* selectionSprite = nullptr;

	std::vector<sf::Sprite*> reachablePositionSprites;

	sf::Text playingPlayerTxt;
	sf::Text matPlayerText;
	sf::Text matPlayerText2;

	sf::Font* mainFont;
};

