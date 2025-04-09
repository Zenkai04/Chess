#include "Pawn.h"
#include <math.h>

Pawn::Pawn(int team, sf::Vector2i position) : Piece(PieceType::PawnPiece, position, 1, team) {
	sf::Texture* texture = ResourceManager::getInstance()->getPieceTexture(team, pieceType);

	sprite = new sf::Sprite();
	sprite->setTexture(*texture);
	sf::Vector2u textureSize = texture->getSize();
	float xRatio = caseSize.x / textureSize.x * 1;
	float yRatio = caseSize.y / textureSize.y * 1;
	sprite->setScale(xRatio, yRatio);
	score = 100;
}


bool Pawn::canMoveToPosition(sf::Vector2i testPosition, Piece*** pieces) const {
	for (unsigned int i = 0; i < reachablePositions.size(); ++i) {
		if (reachablePositions[i] == testPosition) {
			return true;
		}
	}
	return false;
}

void Pawn::updateReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	reachablePositions = computeReachablePositions(pieces, reachableCases, playingPlayer);
}

std::vector<sf::Vector2i> Pawn::computeReachablePositions(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	int width = sizeof(pieces);
	int height = sizeof(pieces[0]);

	std::vector<sf::Vector2i> newReachablePositions;

	int side = team == 1 ? 1 : -1;

	int testX = position.x;
	int testY = position.y + side;


	if (isInMap(testX, testY, width, height)) {
		if (!pieces[testX][testY]) {
			sf::Vector2i testPos = sf::Vector2i(testX, testY);
			if (team != playingPlayer->getTeam() || isKingSafeIfMoveToThisPosition(testPos, pieces, reachableCases, playingPlayer))
				newReachablePositions.push_back(testPos);

			testX = position.x;
			testY = position.y + side * 2;
			testPos = sf::Vector2i(testX, testY);
			if ((side == 1 && position.y == 1 || side == -1 && position.y == 6) && isInMap(testX, testY, width, height)) {
				if (!pieces[testX][testY]) {
					if (team != playingPlayer->getTeam() || isKingSafeIfMoveToThisPosition(testPos, pieces, reachableCases, playingPlayer))
						newReachablePositions.push_back(testPos);
				}
			}
		}
	}

	testX = position.x - 1;
	testY = position.y + side;
	sf::Vector2i testPos = sf::Vector2i(testX, testY);
	if (isInMap(testX, testY, width, height)) {
		if (pieces[testX][testY] && pieces[testX][testY]->getTeam() != team) {
			if (team != playingPlayer->getTeam() || isKingSafeIfMoveToThisPosition(testPos, pieces, reachableCases, playingPlayer))
				newReachablePositions.push_back(testPos);
		}
	}
	testX = position.x + 1;
	testY = position.y + side;
	testPos = sf::Vector2i(testX, testY);
	if (isInMap(testX, testY, width, height)) {
		if (pieces[testX][testY] && pieces[testX][testY]->getTeam() != team) {
			if (team != playingPlayer->getTeam() || isKingSafeIfMoveToThisPosition(testPos, pieces, reachableCases, playingPlayer))
				newReachablePositions.push_back(testPos);
		}
	}
	return newReachablePositions;
}

bool Pawn::isInMap(int x, int y, int width, int height) {
	return (x >= 0 && x < width&& y >= 0 && y < height);
}


void Pawn::checkEnPassant(Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer, sf::Vector2i advancedPosition) {
	if (playingPlayer->getTeam() != team) return;
	int side = team == 1 ? 1 : -1;
	int baseY = team == 1 ? 4 : 3;
	if (position.y == baseY) { // pion à la bonne place en y
		if (abs(advancedPosition.x - position.x) == 1) { // pion à la bonne place en x
			if (!pieces[advancedPosition.x][advancedPosition.y + side]) { // pas de pion à la place cible
				sf::Vector2i targetPos = sf::Vector2i(advancedPosition.x, advancedPosition.y + side);
				if (isKingSafeIfMoveEnPassant(
					targetPos, advancedPosition, pieces, reachableCases, playingPlayer)
					) {
					reachablePositions.push_back(targetPos);
				}
			}
		}
	}
}

bool Pawn::isKingSafeIfMoveEnPassant(sf::Vector2i targetPosition, sf::Vector2i eatenPawnPosition, Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	int width = sizeof(pieces);
	int height = sizeof(pieces[0]);

	Piece* eatenPiece = pieces[eatenPawnPosition.x][eatenPawnPosition.y];
	pieces[eatenPawnPosition.x][eatenPawnPosition.y] = nullptr;
	pieces[targetPosition.x][targetPosition.y] = pieces[position.x][position.y];
	pieces[position.x][position.y] = nullptr;

	sf::Vector2i kingPosition = playingPlayer->king->getPosition();

	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < width; ++y) {
			Piece* piece = pieces[x][y];
			if (!piece) continue;
			sf::Vector2i piecePos = piece->getPosition();
			if (piece->getTeam() != team && piecePos != targetPosition && piecePos != eatenPawnPosition) {
				std::vector<sf::Vector2i> reachablePositions = piece->computeReachablePositions(pieces, reachableCases, playingPlayer);
				for (unsigned int j = 0; j < reachablePositions.size(); ++j) {
					if (reachablePositions[j] == kingPosition) {
						pieces[targetPosition.x][targetPosition.y] = nullptr;
						pieces[eatenPawnPosition.x][eatenPawnPosition.y] = eatenPiece;
						pieces[position.x][position.y] = this;
						return false;
					}
				}
			}
		}
	}

	pieces[targetPosition.x][targetPosition.y] = nullptr;
	pieces[eatenPawnPosition.x][eatenPawnPosition.y] = eatenPiece;
	pieces[position.x][position.y] = this;
	return true;
}