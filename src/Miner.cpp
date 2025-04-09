#include "Miner.h"

Miner::Miner(int team, sf::Vector2i position) : Piece(PieceType::MinerPiece, position, 1, team) {
	sf::Texture* texture = ResourceManager::getInstance()->getPieceTexture(team, pieceType);

	sprite = new sf::Sprite();
	sprite->setTexture(*texture);
	sf::Vector2u textureSize = texture->getSize();
	float xRatio = caseSize.x / textureSize.x * 1;
	float yRatio = caseSize.y / textureSize.y * 1;
	sprite->setScale(xRatio, yRatio);
	score = 400;
}


bool Miner::canMoveToPosition(sf::Vector2i testPosition, Piece*** pieces) const {
	for (unsigned int i = 0; i < reachablePositions.size(); ++i) {
		if (reachablePositions[i] == testPosition) {
			return true;
		}
	}
	return false;
}


void Miner::updateReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	reachablePositions = computeReachablePositions(pieces, reachableCases, playingPlayer);
}

std::vector<sf::Vector2i> Miner::computeReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	int width = sizeof(pieces);
	int height = sizeof(pieces[0]);

	std::vector<sf::Vector2i> newReachablePositions;

	float maxDistance = 5;

	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < width; ++y) {
			sf::Vector2i testPos = sf::Vector2i(x, y);

			float distance = sqrt(pow((testPos - position).x, 2) + pow((testPos - position).y, 2));

			if (distance > maxDistance) continue;
			
			if (pieces[testPos.x][testPos.y]) {
				float movementDistance = abs((testPos - position).x) + abs((testPos - position).y);
				if (movementDistance == 1 && pieces[testPos.x][testPos.y]->getTeam() != team) {
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

	return newReachablePositions;
}


bool Miner::isInMap(int x, int y, int width, int height) {
	return (x >= 0 && x < width&& y >= 0 && y < height);
}