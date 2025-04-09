#include "Wizard.h"

Wizard::Wizard(int team, sf::Vector2i position) : Piece(PieceType::WizardPiece, position, 1, team) {
	sf::Texture* texture = ResourceManager::getInstance()->getPieceTexture(team, pieceType);

	sprite = new sf::Sprite();
	sprite->setTexture(*texture);
	sf::Vector2u textureSize = texture->getSize();
	float xRatio = caseSize.x / textureSize.x * 1;
	float yRatio = caseSize.y / textureSize.y * 1;
	sprite->setScale(xRatio, yRatio);
	score = 450;
}


bool Wizard::canMoveToPosition(sf::Vector2i testPosition, Piece*** pieces) const {
	for (unsigned int i = 0; i < reachablePositions.size(); ++i) {
		if (reachablePositions[i] == testPosition) {
			return true;
		}
	}
	return false;
}


void Wizard::updateReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	reachablePositions = computeReachablePositions(pieces, reachableCases, playingPlayer);
}

std::vector<sf::Vector2i> Wizard::computeReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	int width = sizeof(pieces);
	int height = sizeof(pieces[0]);

	std::vector<sf::Vector2i> newReachablePositions;

	float maxDistance = 6;

	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < width; ++y) {
			sf::Vector2i testPos = sf::Vector2i(x, y);

			float distance = sqrt(pow((testPos - position).x, 2) + pow((testPos - position).y, 2));

			if (distance > maxDistance || distance == 0) continue;

			if (pieces[testPos.x][testPos.y]) {
				if (pieces[testPos.x][testPos.y]->getTeam() == team) {
					if (team != playingPlayer->getTeam() || isKingSafeIfMoveToThisPosition(testPos, pieces, reachableCases, playingPlayer))
						newReachablePositions.push_back(testPos);
				}
				else if(distance == 1) {
					if (team != playingPlayer->getTeam() || isKingSafeIfMoveToThisPosition(testPos, pieces, reachableCases, playingPlayer))
						newReachablePositions.push_back(testPos);
				}
			}
			else if(distance == 1) {
				if (team != playingPlayer->getTeam() || isKingSafeIfMoveToThisPosition(testPos, pieces, reachableCases, playingPlayer))
					newReachablePositions.push_back(testPos);
			}
		}
	}

	return newReachablePositions;
}


bool Wizard::isInMap(int x, int y, int width, int height) {
	return (x >= 0 && x < width&& y >= 0 && y < height);
}