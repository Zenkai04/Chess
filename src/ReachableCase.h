#pragma once

class Piece;

#include "Piece.h"
#include <vector>

class ReachableCase
{
public:
	ReachableCase();

	std::vector<Piece*>& getPieces();
	bool canPieceReachPos(Piece* piece);
	bool canBeReachedByPieceFromDiffTeam(int team);
	void addPiece(Piece* piece);
	void removePiece(Piece* piece);
	void replacePiece(Piece* basePiece, Piece* newPiece);

private:
	std::vector<Piece*> pieces;


};

