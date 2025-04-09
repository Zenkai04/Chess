#pragma once

class Board;

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>

#include "Piece.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Pawn.h"
#include "Queen.h"
#include "King.h"
#include "Miner.h"
#include "Archer.h"
#include "Wizard.h"

#include "Player.h"
#include "CaseColor.h"

#include "GraphicManager.h"
#include "ReachableCase.h"

#include "BoardType.h"

class Board
{
public:

	Board(BoardType type, Player* player1, Player* player2);
	~Board();

	bool canMoveSelectedPieceToPosition(sf::Vector2i targetPosition);
	bool selectedPieceAction(sf::Vector2i position);
	bool isKingMat(King* king);
	void updateAllReachablePositions();


	/////////////////////////   GET
	CaseColor getCaseColor(sf::Vector2i position);
	CaseColor getCaseColor(int x, int y);
	std::vector<sf::Vector2i>& getChangedCases();
	int getWidth();
	int getHeight();
	Piece*** getPieces();
	Piece* getSelectedPiece();
	bool isInBoard(sf::Vector2i position);
	bool isInBoard(int x, int y);
	Player* getPlayer1();
	Player* getPlayer2();
	King* getPlayer1King();
	King* getPlayer2King();
	bool isWaitingPromotion();

	/////////////////////////   SET
	void selectPiece(sf::Vector2i position);
	void unselectPiece();
	void setPositionUnderMouse(sf::Vector2i position);

	bool hasChanged = false;
	std::vector<sf::Vector2i> changedCases;
	std::vector<sf::Vector2i> changedPiecesPositions;

private:
	void moveSelectedPiece(sf::Vector2i targetPosition);
	void udpateReachablePositions(Piece* piece);
	void initStandardBoard();
	void initSpecialBoard();

	Piece*** pieces;
	Piece* selectedPiece = nullptr;
	sf::Vector2i positionUnderMouse;
	Player* player1;
	King* player1King = nullptr;
	Player* player2;
	King* player2King = nullptr;

	ReachableCase** reachableCases = nullptr;

	int width;
	int height;

	bool enPassantPossibleNextTurn = false;
	sf::Vector2i enPassantPiecePosition;

	bool waitingPromotion = false;
	sf::Vector2i promotionPosition;
};

