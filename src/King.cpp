#include "King.h"

King::King(int team, sf::Vector2i position) : Piece(PieceType::KingPiece, position, 1, team) {
	sf::Texture* texture = ResourceManager::getInstance()->getPieceTexture(team, pieceType);

	sprite = new sf::Sprite();
	sprite->setTexture(*texture);
	sf::Vector2u textureSize = texture->getSize();
	float xRatio = caseSize.x / textureSize.x * 1;
	float yRatio = caseSize.y / textureSize.y * 1;
	sprite->setScale(xRatio, yRatio);
	score = 10000;
}


bool King::canMoveToPosition(sf::Vector2i testPosition, Piece*** pieces) const {
	for (unsigned int i = 0; i < reachablePositions.size(); ++i) {
		if (reachablePositions[i] == testPosition) {
			return true;
		}
	}
	return false;
}


void King::updateReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	reachablePositions = computeReachablePositions(pieces, reachableCases, playingPlayer);
}

std::vector<sf::Vector2i> King::computeReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	int width = sizeof(pieces);
	int height = sizeof(pieces[0]);

	std::vector<sf::Vector2i> newReachablePositions;

	for (int dx = -1; dx < 2; dx++) {
		for (int dy = -1; dy < 2; dy++) {
			if (dx == 0 && dy == 0) continue;
			sf::Vector2i testPos = sf::Vector2i(position.x + dx, position.y + dy);
			if (isInMap(testPos.x, testPos.y, width, height)) {
				if (pieces[testPos.x][testPos.y]) {
					if (pieces[testPos.x][testPos.y]->getTeam() != team) {
						if (team != playingPlayer->getTeam() || isKingSafeIfMoveToThisPosition(testPos, pieces, reachableCases, playingPlayer))
							newReachablePositions.push_back(testPos);
					}
				}
				else {
					if (team != playingPlayer->getTeam() || isKingSafeIfMoveToThisPosition(testPos, pieces, reachableCases, playingPlayer))
						newReachablePositions.push_back(testPos);
				}
			}
		}
	}

	// roque
	if (team == playingPlayer->getTeam() && !alreadyMoved) {
		// right
		if (!pieces[position.x + 1][position.y] && !pieces[position.x + 2][position.y]) {
			Piece* testPiece = pieces[position.x + 3][position.y];
			if (testPiece && testPiece->getType() == RookPiece && !testPiece->hasAlreadyMoved()) {
				if (!reachableCases[position.x + 1][position.y].canBeReachedByPieceFromDiffTeam(team) &&
					!reachableCases[position.x + 2][position.y].canBeReachedByPieceFromDiffTeam(team)
					) {
					newReachablePositions.push_back(sf::Vector2i(position.x + 3, position.y));
				}
			}
		}

		// left
		if (!pieces[position.x - 1][position.y] && !pieces[position.x - 2][position.y] && !pieces[position.x - 3][position.y]) {
			Piece* testPiece = pieces[position.x - 4][position.y];
			if (testPiece && testPiece->getType() == RookPiece && !testPiece->hasAlreadyMoved()) {
				if (!reachableCases[position.x - 1][position.y].canBeReachedByPieceFromDiffTeam(team) &&
					!reachableCases[position.x - 2][position.y].canBeReachedByPieceFromDiffTeam(team)
					) {
					newReachablePositions.push_back(sf::Vector2i(position.x - 4, position.y));
				}
			}
		}
	}

	return newReachablePositions;
}


bool King::isInMap(int x, int y, int width, int height) {
	return (x >= 0 && x < width&& y >= 0 && y < height);
}