#pragma once

class Piece;
class AiBoard;

#include <vector>
#include "Piece.h"
#include "AiBoard.h"
#include "ControllerType.h"

class Player
{
public:	
	Player(int team, ControllerType controllerType);
	~Player();

	int getTeam();

	ControllerType getControllerType();

	Piece* king;
	bool playing = false;

	Move getBestMove(Piece*** pieces, int width, int height);

	void setAiLevel(int level);

protected:
	BestMove createBestMove(int value, Move move);
	BestMove calcBestMove(int depth, AiBoard* game, int alpha = INT_MIN,
		int beta = INT_MAX, bool isMaximizingPlayer = true, bool firstLoop = true);

	int team;
	ControllerType controllerType;
	std::vector<Piece*> pieces;
	std::vector<Piece*> deadPieces;

	int aiLevel = 5;
};

