#include "Archer.h"

Archer::Archer(int team, sf::Vector2i position) : Piece(PieceType::ArcherPiece, position, 1, team) {
	sf::Texture* texture = ResourceManager::getInstance()->getPieceTexture(team, pieceType);

	sprite = new sf::Sprite();
	sprite->setTexture(*texture);
	sf::Vector2u textureSize = texture->getSize();
	float xRatio = caseSize.x / textureSize.x * 1;
	float yRatio = caseSize.y / textureSize.y * 1;
	sprite->setScale(xRatio, yRatio);
	score = 375;
}


bool Archer::canMoveToPosition(sf::Vector2i testPosition, Piece*** pieces) const {
	for (unsigned int i = 0; i < reachablePositions.size(); ++i) {
		if (reachablePositions[i] == testPosition) {
			return true;
		}
	}
	return false;
}


void Archer::updateReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	reachablePositions = computeReachablePositions(pieces, reachableCases, playingPlayer);
}

std::vector<sf::Vector2i> Archer::computeReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	int width = sizeof(pieces);
	int height = sizeof(pieces[0]);

	int range = 4;

	std::vector<sf::Vector2i> newReachablePositions;

	for (int x = -range; x <= range; ++x) {
		for (int y = -range; y <= range; ++y) {
			sf::Vector2i testPos = sf::Vector2i(x, y) + position;

			if (isInMap(testPos.x, testPos.y, width, height)) {
				float distance = sqrt(pow((testPos - position).x, 2) + pow((testPos - position).y, 2));
				if (distance > 2 && distance <= 3 && pieces[testPos.x][testPos.y] && pieces[testPos.x][testPos.y]->getTeam() != team) {
					if (team != playingPlayer->getTeam() || isKingSafeIfMoveToThisPosition(testPos, pieces, reachableCases, playingPlayer))
						newReachablePositions.push_back(testPos);
				}
				
				if(distance == 1) {
					if (!pieces[testPos.x][testPos.y] || pieces[testPos.x][testPos.y]->getTeam() != team) {
						if (team != playingPlayer->getTeam() || isKingSafeIfMoveToThisPosition(testPos, pieces, reachableCases, playingPlayer))
							newReachablePositions.push_back(testPos);
					}
				}
			}
		}
	}

	return newReachablePositions;
}


bool Archer::isInMap(int x, int y, int width, int height) {
	return (x >= 0 && x < width&& y >= 0 && y < height);
}