#pragma once

class Pawn;

#include <SFML/System/Vector2.hpp>
#include "Piece.h"
#include "ResourceManager.h"
#include "GraphicManager.h"

class Pawn : public Piece
{
public:
	Pawn(int team, sf::Vector2i position);
	bool canMoveToPosition(sf::Vector2i testPosition, Piece*** pieces) const override;
	void updateReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) override;
	std::vector<sf::Vector2i> computeReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) override;

	void checkEnPassant(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer, sf::Vector2i advancedPosition);

private:
	bool isKingSafeIfMoveEnPassant(sf::Vector2i targetPosition, sf::Vector2i eatenPawnPosition, Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer);
	bool isInMap(int x, int y, int width, int height);
};

