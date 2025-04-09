#include "Bishop.h"


Bishop::Bishop(int team, sf::Vector2i position): Piece(PieceType::BishopPiece, position, 3, team) {
	sf::Texture* texture = ResourceManager::getInstance()->getPieceTexture(team, pieceType);

	sprite = new sf::Sprite();
	sprite->setTexture(*texture);
	sf::Vector2u textureSize = texture->getSize();
	float xRatio = caseSize.x / textureSize.x * 1;
	float yRatio = caseSize.y / textureSize.y * 1;
	sprite->setScale(xRatio, yRatio);
	score = 351;
}


bool Bishop::canMoveToPosition(sf::Vector2i testPosition, Piece*** pieces) const {
	for (unsigned int i = 0; i < reachablePositions.size(); ++i) {
		if (reachablePositions[i] == testPosition) {
			return true;
		}
	}
	return false;
}

void Bishop::updateReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	reachablePositions = computeReachablePositions(pieces, reachableCases, playingPlayer);
}

std::vector<sf::Vector2i> Bishop::computeReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	int width = sizeof(pieces);
	int height = sizeof(pieces[0]);

	std::vector<sf::Vector2i> newReachablePositions;

	for (unsigned int i = 0; i < 4; ++i) {
		int dx = 1 - 2 * (i % 2);
		int dy = 1 - 2 * (i / 2);

		sf::Vector2i testPos = sf::Vector2i(position.x + dx, position.y + dy);
		while (isInMap(testPos.x, testPos.y, width, height)) {
			if (pieces[testPos.x][testPos.y]) {
				if (pieces[testPos.x][testPos.y]->getTeam() != team) {
					if (team != playingPlayer->getTeam() || isKingSafeIfMoveToThisPosition(testPos, pieces, reachableCases, playingPlayer))
						newReachablePositions.push_back(testPos);
				}
				break;
			}
			else {
				if (team != playingPlayer->getTeam() || isKingSafeIfMoveToThisPosition(testPos, pieces, reachableCases, playingPlayer))
					newReachablePositions.push_back(testPos);
			}
			testPos = sf::Vector2i(testPos.x + dx, testPos.y + dy);
		}
	}
	return newReachablePositions;
}


bool Bishop::isInMap(int x, int y, int width, int height) {
	return (x >= 0 && x < width&& y >= 0 && y < height);
}