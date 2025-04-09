#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "PieceType.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	static ResourceManager* getInstance();

	sf::Texture* getPieceTexture(int team, PieceType pieceType);
	sf::Texture* getSelectionTexture();
	sf::Texture* getReachablePositionTexture();

private:
	void loadPieceTextures();

	static ResourceManager* instance;

	sf::Texture* w_king = nullptr;
	sf::Texture* b_king = nullptr;
	sf::Texture* w_queen = nullptr;
	sf::Texture* b_queen = nullptr;
	sf::Texture* w_rook = nullptr;
	sf::Texture* b_rook = nullptr;
	sf::Texture* w_knight = nullptr;
	sf::Texture* b_knight = nullptr;
	sf::Texture* w_bishop = nullptr;
	sf::Texture* b_bishop = nullptr;
	sf::Texture* w_pawn = nullptr;
	sf::Texture* b_pawn = nullptr;

	sf::Texture* w_wizard = nullptr;
	sf::Texture* b_wizard = nullptr;
	sf::Texture* w_miner = nullptr;
	sf::Texture* b_miner = nullptr;
	sf::Texture* w_archer = nullptr;
	sf::Texture* b_archer = nullptr;

	sf::Texture* selectionTexture = nullptr;
	sf::Texture* reachablePositionTexture = nullptr;

	std::string assetsDir = "assets/";
	std::string graphicsDir = "assets/graphics/";
};

