#pragma once

class Archer;

#include <SFML/System/Vector2.hpp>
#include "Piece.h"
#include "ResourceManager.h"
#include "GraphicManager.h"

class Archer : public Piece
{
public:
	Archer(int team, sf::Vector2i position);
	bool canMoveToPosition(sf::Vector2i testPosition, Piece*** pieces) const override;
	void updateReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) override;
	std::vector<sf::Vector2i> computeReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) override;

private:
	bool isInMap(int x, int y, int width, int height);
};
