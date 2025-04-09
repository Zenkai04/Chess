#pragma once

class GameManager;

#include <SFML/Graphics.hpp>
#include <iostream>
#include "ResourceManager.h"
#include "GraphicManager.h"
#include "Board.h"
#include "InputManager.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

class GameManager
{
public:
	GameManager(BoardType boardType);
	~GameManager();
	void init();

	bool isInBoard(sf::Vector2i screenPosition);
	sf::Vector2i getPositionInBoard(sf::Vector2i screenPosition);

private:
	void gameLoop();
	void endPlayerTurn();
	void checkMat();
	void manageLeftClickRelease();
	void manageRightClickRelease();
	void updatePositionUnderMouse();

	sf::RenderWindow* window = nullptr;
	GraphicManager* graphicManager = nullptr;
	InputManager* inputManager = nullptr;
	ResourceManager* resourceManager = nullptr;
	Board* board = nullptr;
	Player* player1 = nullptr;
	King* player1King = nullptr;
	Player* player2 = nullptr;
	Player* playingPlayer = nullptr;

	bool mat = false;
	Player* matPlayer = nullptr;

	int turnCount = 0;
};

