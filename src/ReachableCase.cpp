#include "ReachableCase.h"

ReachableCase::ReachableCase() {

}

std::vector<Piece*>& ReachableCase::getPieces() {
	return pieces;
}


bool ReachableCase::canPieceReachPos(Piece* piece) {
	for (unsigned int i = 0; i < pieces.size(); ++i) {
		if (pieces[i] == piece) return true;
	}
	return false;
}

bool ReachableCase::canBeReachedByPieceFromDiffTeam(int team) {
	for (unsigned int i = 0; i < pieces.size(); ++i) {
		if (pieces[i]->getTeam() != team) return true;
	}
	return false;
}


void ReachableCase::addPiece(Piece* piece) {
	if (!canPieceReachPos(piece)) {
		pieces.push_back(piece);
	}
}

void ReachableCase::removePiece(Piece* piece) {
	for (std::vector<Piece*>::iterator it = pieces.begin(); it != pieces.end(); ++it) {
		if ((*it) == piece) {
			pieces.erase(it);
			break;
		}
	}
}

void ReachableCase::replacePiece(Piece* basePiece, Piece* newPiece) {
	for (std::vector<Piece*>::iterator it = pieces.begin(); it != pieces.end(); ++it) {
		if ((*it) == basePiece) {
			*it = newPiece;
		}
	}
}