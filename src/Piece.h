#pragma once

class Piece;
class Player;
class ReachableCase;

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "ReachableCase.h"
#include "PieceType.h"
#include "Player.h"

class Piece
{
public:
	Piece(PieceType pieceType, sf::Vector2i position, int score, int team);
	~Piece();
	virtual bool canMoveToPosition(sf::Vector2i testPosition, Piece*** pieces) const = 0;
	virtual void updateReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) = 0;

	//////////////////////  GET
	sf::Vector2i getPosition();
	sf::Sprite* getSprite();
	int getTeam();
	PieceType getType();
	std::vector<sf::Vector2i>& getReachablePositions();
	virtual std::vector<sf::Vector2i> computeReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) = 0;
	bool hasAlreadyMoved();
	int getScore();

	//////////////////////  SET
	void setPosition(sf::Vector2i position);


protected:
	bool isKingSafeIfMoveToThisPosition(sf::Vector2i testPosition, Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer);
	bool isInMap(int x, int y, int width, int height);

	int team;
	PieceType pieceType;
	sf::Vector2i position;
	int score;
	bool alreadyMoved = false;
	sf::Sprite* sprite;
	std::vector<sf::Vector2i> reachablePositions;
};

