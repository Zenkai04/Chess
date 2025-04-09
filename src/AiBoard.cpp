#include "AiBoard.h"
#include <math.h>
#include <iostream>

const int MINER_SCORE = 400;
const int ARCHER_SCORE = 375;
const int WIZARD_SCORE = 450;

AiBoard::AiBoard(int width, int height) : width(width), height(height) {

}

AiBoard::~AiBoard() {
	cleanBoard(board);
	for (unsigned int i = 0; i < previousStates.size(); ++i) {
		cleanBoard(previousStates[i]);
	}
}

void AiBoard::update(int** newBoard, int playerTeam) {
	cleanBoard(board);
	for (unsigned int i = 0; i < previousStates.size(); ++i) {
		cleanBoard(previousStates[i]);
	}
	previousStates.clear();

	board = newBoard;
	this->playerTeam = playerTeam;
}

std::vector<Move> AiBoard::getPossibleMoves(int playerFactor) {
	std::vector<Move> possibleMoves = std::vector<Move>();
	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < height; ++y) {
			// signifie que la piece est de l'equipe du joueur indiqué
			if (board[x][y] * playerFactor > 0) {
				std::vector<Move> pieceMoves = getPieceMoves(sf::Vector2i(x, y), board[x][y]);
				for (unsigned int i = 0; i < pieceMoves.size(); ++i) {
					possibleMoves.push_back(pieceMoves[i]);
				}
			}
		}
	}

	return possibleMoves;
}

void AiBoard::move(Move movement) {
	previousStates.push_back(board);

	int** previousBoard = board;

	board = new int* [width];

	for (unsigned int x = 0; x < width; ++x) {
		board[x] = new int[height];
		for (unsigned int y = 0; y < height; ++y) {
			board[x][y] = previousBoard[x][y];
		}
	}

	int pieceScore = board[movement.from.x][movement.from.y];
	int targetScore = board[movement.to.x][movement.to.y];

	if (abs(pieceScore) == WIZARD_SCORE && targetScore * pieceScore > 0) {
		board[movement.to.x][movement.to.y] = pieceScore;
		board[movement.from.x][movement.from.y] = targetScore;
	}
	else {
		board[movement.to.x][movement.to.y] = pieceScore;
		board[movement.from.x][movement.from.y] = 0;

		// pawn
		if (abs(pieceScore) == 100 && (movement.to.y == 0 || movement.to.y == height - 1)) {
			//promotion
			// dame
			board[movement.to.x][movement.to.y] = 1000;
		}
	}
}

void AiBoard::undo() {
	int size = previousStates.size();
	if (size > 0) {
		cleanBoard(board);
		board = previousStates[size - 1];
		previousStates.pop_back();
	}
}

void AiBoard::cleanBoard(int** board) {
	if (board) {
		for (unsigned int i = 0; i < width; ++i) {
			delete[] board[i];
		}
		delete[] board;
	}
}

bool AiBoard::isInMap(int x, int y, int width, int height) {
	return (x >= 0 && x < width&& y >= 0 && y < height);
}

bool AiBoard::kingAlive(int playerFactor) {
	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < height; ++y) {
			// signifie que la piece est de l'equipe du joueur indiqué
			if (board[x][y] * playerFactor > 0 && abs(board[x][y]) == 10000) {
				return true;
			}
		}
	}
	return false;
}

std::vector<Move> AiBoard::getPieceMoves(sf::Vector2i position, int score) {
	switch (abs(score)) {
	case 100:
		return getPawnMoves(position, score);
	case MINER_SCORE:
		return getMinerMoves(position, score);
	case ARCHER_SCORE:
		return getArcherMoves(position, score);
	case 350:
		return getKnightMoves(position, score);
	case 351:
		return getBishopMoves(position, score);
	case 525:
		return getRookMoves(position, score);
	case 1000:
		return getQueenMoves(position, score);
	case 10000:
		return getKingMoves(position, score);
	}
	return std::vector<Move>();
}


std::vector<Move> AiBoard::getPawnMoves(sf::Vector2i position, int score) {
	std::vector<Move> moves = std::vector<Move>();

	int side = (playerTeam == 1 ? 1 : -1) * (score > 0 ? 1 : -1);

	int testX = position.x;
	int testY = position.y + side;

	if (isInMap(testX, testY, width, height)) {
		if (!board[testX][testY]) {
			sf::Vector2i testPos = sf::Vector2i(testX, testY);
			moves.push_back(Move(position, testPos));

			testX = position.x;
			testY = position.y + side * 2;
			testPos = sf::Vector2i(testX, testY);
			if ((side == 1 && position.y == 1 || side == -1 && position.y == 6) && isInMap(testX, testY, width, height)) {
				if (!board[testX][testY]) {
					moves.push_back(Move(position, testPos));
				}
			}
		}
	}

	testX = position.x - 1;
	testY = position.y + side;
	sf::Vector2i testPos = sf::Vector2i(testX, testY);
	if (isInMap(testX, testY, width, height)) {
		if (board[testX][testY] && board[testX][testY] * score < 0) {
			moves.push_back(Move(position, testPos));
		}
	}
	testX = position.x + 1;
	testY = position.y + side;
	testPos = sf::Vector2i(testX, testY);
	if (isInMap(testX, testY, width, height)) {
		if (board[testX][testY] && board[testX][testY] * score < 0) {
			moves.push_back(Move(position, testPos));
		}
	}
	return moves;
}

std::vector<Move> AiBoard::getKnightMoves(sf::Vector2i position, int score) {
	std::vector<Move> moves = std::vector<Move>();

	for (unsigned int i = 0; i < 2; ++i) {
		for (unsigned int j = 0; j < 2; ++j) {

			int testX = position.x - 2 + 4 * j;
			int testY = position.y - 1 + 2 * i;
			sf::Vector2i testPos = sf::Vector2i(testX, testY);
			if (isInMap(testX, testY, width, height)) {
				if (!board[testX][testY]) {
					moves.push_back(Move(position, testPos));
				}
				else if (board[testX][testY] * score < 0) {
					moves.push_back(Move(position, testPos));
				}
			}

			testX = position.x - 1 + 2 * j;
			testY = position.y - 2 + 4 * i;
			testPos = sf::Vector2i(testX, testY);
			if (isInMap(testX, testY, width, height)) {
				if (!board[testX][testY]) {
					moves.push_back(Move(position, testPos));
				}
				else if (board[testX][testY] * score < 0) {
					moves.push_back(Move(position, testPos));
				}
			}
		}
	}

	return moves;
}

std::vector<Move> AiBoard::getBishopMoves(sf::Vector2i position, int score) {
	std::vector<Move> moves = std::vector<Move>();

	for (unsigned int i = 0; i < 4; ++i) {
		int dx = 1 - 2 * (i % 2);
		int dy = 1 - 2 * (i / 2);

		sf::Vector2i testPos = sf::Vector2i(position.x + dx, position.y + dy);
		while (isInMap(testPos.x, testPos.y, width, height)) {
			if (board[testPos.x][testPos.y]) {
				if (board[testPos.x][testPos.y] * score < 0) {
					moves.push_back(Move(position, testPos));
				}
				break;
			}
			else {
				moves.push_back(Move(position, testPos));
			}
			testPos = sf::Vector2i(testPos.x + dx, testPos.y + dy);
		}
	}

	return moves;
}

std::vector<Move> AiBoard::getRookMoves(sf::Vector2i position, int score) {
	std::vector<Move> moves = std::vector<Move>();

	for (int dx = -1; dx < 2; dx += 2) {
		sf::Vector2i testPos = sf::Vector2i(position.x + dx, position.y);
		while (isInMap(testPos.x, testPos.y, width, height)) {
			if (board[testPos.x][testPos.y]) {
				if (board[testPos.x][testPos.y] * score < 0) {
					moves.push_back(Move(position, testPos));
				}
				break;
			}
			else {
				moves.push_back(Move(position, testPos));
			}
			testPos = sf::Vector2i(testPos.x + dx, testPos.y);
		}
	}

	for (int dy = -1; dy < 2; dy += 2) {
		sf::Vector2i testPos = sf::Vector2i(position.x, position.y + dy);
		while (isInMap(testPos.x, testPos.y, width, height)) {
			if (board[testPos.x][testPos.y]) {
				if (board[testPos.x][testPos.y] * score < 0) {
					moves.push_back(Move(position, testPos));
				}
				break;
			}
			else {
				moves.push_back(Move(position, testPos));
			}
			testPos = sf::Vector2i(testPos.x, testPos.y + dy);
		}
	}

	return moves;
}

std::vector<Move> AiBoard::getQueenMoves(sf::Vector2i position, int score) {
	std::vector<Move> bishopMoves = getBishopMoves(position, score);
	std::vector<Move> rookMoves = getRookMoves(position, score);

	int totalSize = bishopMoves.size() + rookMoves.size();

	std::vector<Move> moves = std::vector<Move>();

	for (unsigned int i = 0; i < bishopMoves.size(); ++i) {
		moves.push_back(bishopMoves[i]);
	}
	for (unsigned int i = 0; i < rookMoves.size(); ++i) {
		moves.push_back(rookMoves[i]);
	}

	return moves;
}

std::vector<Move> AiBoard::getKingMoves(sf::Vector2i position, int score) {
	std::vector<Move> moves = std::vector<Move>();

	for (int dx = -1; dx < 2; dx++) {
		for (int dy = -1; dy < 2; dy++) {
			if (dx == 0 && dy == 0) continue;
			sf::Vector2i testPos = sf::Vector2i(position.x + dx, position.y + dy);
			if (isInMap(testPos.x, testPos.y, width, height)) {
				if (board[testPos.x][testPos.y]) {
					if (board[testPos.x][testPos.y] * score < 0) {
						moves.push_back(Move(position, testPos));
					}
				}
				else {
					moves.push_back(Move(position, testPos));
				}
			}
		}
	}

	// roque
	/*if (team == playingPlayer->getTeam() && !alreadyMoved) {
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
	}*/

	return moves;
}

std::vector<Move> AiBoard::getMinerMoves(sf::Vector2i position, int score) {
	std::vector<Move> moves = std::vector<Move>();

	float maxDistance = 5;

	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < width; ++y) {
			sf::Vector2i testPos = sf::Vector2i(x, y);

			float distance = sqrt(pow((testPos - position).x, 2) + pow((testPos - position).y, 2));
			if (distance > maxDistance) continue;

			if (board[testPos.x][testPos.y]) {
				float movementDistance = abs((testPos - position).x) + abs((testPos - position).y);
				if (movementDistance == 1 && board[testPos.x][testPos.y] * score < 0) {
					moves.push_back(Move(position, testPos));
				}
			}
			else {
				moves.push_back(Move(position, testPos));
			}
		}
	}

	return moves;
}

std::vector<Move> AiBoard::getArcherMoves(sf::Vector2i position, int score) {
	std::vector<Move> moves = std::vector<Move>();

	int range = 4;

	for (int x = -range; x <= range; ++x) {
		for (int y = -range; y <= range; ++y) {
			sf::Vector2i testPos = sf::Vector2i(x, y) + position;

			if (isInMap(testPos.x, testPos.y, width, height)) {
				float distance = sqrt(pow((testPos - position).x, 2) + pow((testPos - position).y, 2));
				if (distance > 2 && distance <= 3 && board[testPos.x][testPos.y] && board[testPos.x][testPos.y] * score < 0) {
					moves.push_back(Move(position, testPos));
				}

				if (distance == 1) {
					if (!board[testPos.x][testPos.y] || board[testPos.x][testPos.y] * score < 0) {
						moves.push_back(Move(position, testPos));
					}
				}
			}
		}
	}

	return moves;
}

std::vector<Move> AiBoard::getWizardMoves(sf::Vector2i position, int score) {
	std::vector<Move> moves = std::vector<Move>();

	float maxDistance = 6;

	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < width; ++y) {
			sf::Vector2i testPos = sf::Vector2i(x, y);

			float distance = sqrt(pow((testPos - position).x, 2) + pow((testPos - position).y, 2));

			if (distance > maxDistance || distance == 0) continue;

			if (board[testPos.x][testPos.y]) {
				if (board[testPos.x][testPos.y] * score > 0) {
					moves.push_back(Move(position, testPos));
				}
				else if (distance == 1) {
					moves.push_back(Move(position, testPos));
				}
			}
			else if (distance == 1) {
				moves.push_back(Move(position, testPos));
			}
		}
	}

	return moves;
}


int AiBoard::evaluate() {
	int sum = 0;
	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < height; ++y) {
			if(board[x][y] == 351)
				sum += 350;
			else 
				sum += board[x][y];
		}
	}
	return sum;
}



