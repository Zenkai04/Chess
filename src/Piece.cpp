#include "Piece.h"


Piece::Piece(PieceType pieceType, sf::Vector2i position, int score, int team) : 
	pieceType(pieceType), position(position), score(score), team(team)
{

}


Piece::~Piece() {
	if (sprite) delete sprite;
}


bool Piece::isKingSafeIfMoveToThisPosition(sf::Vector2i testPosition, Piece*** pieces, ReachableCase** reachableCases, Player* playingPlayer) {
	int width = sizeof(pieces);
	int height = sizeof(pieces[0]);

	Piece* pieceAtTestPos = pieces[testPosition.x][testPosition.y];
	sf::Vector2i kingPosition = playingPlayer->king->getPosition();

	if (pieceType == WizardPiece && pieceAtTestPos && pieceAtTestPos->getTeam() == team) {
		pieces[testPosition.x][testPosition.y] = pieces[position.x][position.y];
		pieces[position.x][position.y] = pieceAtTestPos;

		if (pieceAtTestPos->getType() == KingPiece) {
			kingPosition = position;
		}

		for (unsigned int x = 0; x < width; ++x) {
			for (unsigned int y = 0; y < width; ++y) {
				Piece* piece = pieces[x][y];
				if (!piece) continue;
				sf::Vector2i piecePos = piece->getPosition();
				if (piece->getTeam() != team && piecePos != testPosition) {
					std::vector<sf::Vector2i> reachablePositions = piece->computeReachablePositions(pieces, reachableCases, playingPlayer);
					for (unsigned int j = 0; j < reachablePositions.size(); ++j) {
						if (reachablePositions[j] == kingPosition) {
							pieces[testPosition.x][testPosition.y] = pieceAtTestPos;
							pieces[position.x][position.y] = this;
							return false;
						}
					}
				}
			}
		}

		pieces[testPosition.x][testPosition.y] = pieceAtTestPos;
		pieces[position.x][position.y] = this;
		return true;
	}
	else {
		pieces[testPosition.x][testPosition.y] = pieces[position.x][position.y];
		pieces[position.x][position.y] = nullptr;

		if (pieceType == KingPiece) {
			kingPosition = testPosition;
		}

		for (unsigned int x = 0; x < width; ++x) {
			for (unsigned int y = 0; y < width; ++y) {
				Piece* piece = pieces[x][y];
				if (!piece) continue;
				sf::Vector2i piecePos = piece->getPosition();
				if (piece->getTeam() != team && piecePos != testPosition) {
					std::vector<sf::Vector2i> reachablePositions = piece->computeReachablePositions(pieces, reachableCases, playingPlayer);
					for (unsigned int j = 0; j < reachablePositions.size(); ++j) {
						if (reachablePositions[j] == kingPosition) {
							pieces[testPosition.x][testPosition.y] = pieceAtTestPos;
							pieces[position.x][position.y] = this;
							return false;
						}
					}
				}
			}
		}

		pieces[testPosition.x][testPosition.y] = pieceAtTestPos;
		pieces[position.x][position.y] = this;
		return true;
	}
}


////////////////////////////////////////////////////////////////
//                          GET                               //
////////////////////////////////////////////////////////////////

sf::Vector2i Piece::getPosition() {
	return position;
}

sf::Sprite* Piece::getSprite() {
	return sprite;
}

std::vector<sf::Vector2i>& Piece::getReachablePositions() {
	return reachablePositions;
}

int Piece::getTeam() {
	return team;
}

PieceType Piece::getType() {
	return pieceType;
}

bool Piece::isInMap(int x, int y, int width, int height) {
	return (x >= 0 && x < width&& y >= 0 && y < height);
}

bool Piece::hasAlreadyMoved() {
	return alreadyMoved;
}

int Piece::getScore() {
	return score;
}

////////////////////////////////////////////////////////////////
//                          SET                               //
////////////////////////////////////////////////////////////////

void Piece::setPosition(sf::Vector2i poistion) {
	this->position = poistion;
	alreadyMoved = true;
}