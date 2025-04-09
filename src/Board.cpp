#include "Board.h"


Board::Board(BoardType type, Player* player1, Player* player2): player1(player1), player2(player2) {
	switch (type) {
	case Standard:
		initStandardBoard();
		break;
	case Special:
		initSpecialBoard();
		break;
	}

	positionUnderMouse = sf::Vector2i(-1, -1);
}

Board::~Board() {
	if (pieces) {
		for (unsigned int x = 0; x < width; ++x) {
			for (unsigned int y = 0; y < height; ++y) {
				if (pieces[x][y]) {
					delete pieces[x][y];
				}
			}
			delete[] pieces[x];
		}

		delete[] pieces;
	}

	if (reachableCases) {
		for (unsigned int x = 0; x < width; ++x) {
			delete[] reachableCases[x];
		}

		delete[] reachableCases;
	}
}


void Board::initStandardBoard() {
	width = 8;
	height = 8;

	pieces = new Piece** [width];
	reachableCases = new ReachableCase* [width];
	for (unsigned int x = 0; x < width; ++x) {
		pieces[x] = new Piece * [height];
		reachableCases[x] = new ReachableCase [height];
		for (unsigned int y = 0; y < height; ++y) {
			pieces[x][y] = nullptr;
		}
	}

	pieces[2][0] = new Bishop(1, sf::Vector2i(2, 0));
	pieces[5][0] = new Bishop(1, sf::Vector2i(5, 0));

	pieces[2][7] = new Bishop(2, sf::Vector2i(2, 7));
	pieces[5][7] = new Bishop(2, sf::Vector2i(5, 7));

	pieces[1][0] = new Knight(1, sf::Vector2i(1, 0));
	pieces[6][0] = new Knight(1, sf::Vector2i(6, 0));

	pieces[1][7] = new Knight(2, sf::Vector2i(1, 7));
	pieces[6][7] = new Knight(2, sf::Vector2i(6, 7));

	pieces[0][0] = new Rook(1, sf::Vector2i(0, 0));
	pieces[7][0] = new Rook(1, sf::Vector2i(7, 0));

	pieces[0][7] = new Rook(2, sf::Vector2i(0, 7));
	pieces[7][7] = new Rook(2, sf::Vector2i(7, 7));

	player1King = new King(1, sf::Vector2i(4, 0));
	player1->king = player1King;
	pieces[4][0] = player1King;
	player2King = new King(2, sf::Vector2i(4, 7));
	pieces[4][7] = player2King;
	player2->king = player2King;

	pieces[3][0] = new Queen(1, sf::Vector2i(3, 0));
	pieces[3][7] = new Queen(2, sf::Vector2i(3, 7));

	for (int x = 0; x < width; ++x) {
		pieces[x][1] = new Pawn(1, sf::Vector2i(x, 1));
		pieces[x][6] = new Pawn(2, sf::Vector2i(x, 6));
	}

	Player* playingPlayer = player1->playing ? player1 : player2;
	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < height; ++y) {
			if (pieces[x][y]) {
				pieces[x][y]->updateReachablePositions(pieces, reachableCases, playingPlayer);
				std::vector<sf::Vector2i> reachablePositions = pieces[x][y]->getReachablePositions();
				for (unsigned int i = 0; i < reachablePositions.size(); ++i) {
					reachableCases[reachablePositions[i].x][reachablePositions[i].y].addPiece(pieces[x][y]);
				}
			}
		}
	}

	hasChanged = true;
}

void Board::initSpecialBoard() {
	width = 8;
	height = 8;

	pieces = new Piece * *[width];
	reachableCases = new ReachableCase * [width];
	for (unsigned int x = 0; x < width; ++x) {
		pieces[x] = new Piece * [height];
		reachableCases[x] = new ReachableCase[height];
		for (unsigned int y = 0; y < height; ++y) {
			pieces[x][y] = nullptr;
		}
	}

	pieces[2][0] = new Miner(1, sf::Vector2i(2, 0));
	pieces[5][0] = new Wizard(1, sf::Vector2i(5, 0));

	pieces[2][7] = new Miner(2, sf::Vector2i(2, 7));
	pieces[5][7] = new Wizard(2, sf::Vector2i(5, 7));

	pieces[1][0] = new Bishop(1, sf::Vector2i(1, 0));
	pieces[6][0] = new Bishop(1, sf::Vector2i(6, 0));

	pieces[1][7] = new Bishop(2, sf::Vector2i(1, 7));
	pieces[6][7] = new Bishop(2, sf::Vector2i(6, 7));

	pieces[0][0] = new Archer(1, sf::Vector2i(0, 0));
	pieces[7][0] = new Archer(1, sf::Vector2i(7, 0));

	pieces[0][7] = new Archer(2, sf::Vector2i(0, 7));
	pieces[7][7] = new Archer(2, sf::Vector2i(7, 7));

	player1King = new King(1, sf::Vector2i(4, 0));
	player1->king = player1King;
	pieces[4][0] = player1King;
	player2King = new King(2, sf::Vector2i(4, 7));
	pieces[4][7] = player2King;
	player2->king = player2King;

	pieces[3][0] = new Queen(1, sf::Vector2i(3, 0));
	pieces[3][7] = new Queen(2, sf::Vector2i(3, 7));

	for (int x = 0; x < width; ++x) {
		pieces[x][1] = new Pawn(1, sf::Vector2i(x, 1));
		pieces[x][6] = new Pawn(2, sf::Vector2i(x, 6));
	}

	Player* playingPlayer = player1->playing ? player1 : player2;
	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < height; ++y) {
			if (pieces[x][y]) {
				pieces[x][y]->updateReachablePositions(pieces, reachableCases, playingPlayer);
				std::vector<sf::Vector2i> reachablePositions = pieces[x][y]->getReachablePositions();
				for (unsigned int i = 0; i < reachablePositions.size(); ++i) {
					reachableCases[reachablePositions[i].x][reachablePositions[i].y].addPiece(pieces[x][y]);
				}
			}
		}
	}

	hasChanged = true;
}


bool Board::canMoveSelectedPieceToPosition(sf::Vector2i targetPosition) {
	return true;
}


bool Board::selectedPieceAction(sf::Vector2i position) {
	Player* playingPlayer = player1->playing ? player1 : player2;
	if (selectedPiece && selectedPiece->getTeam() == playingPlayer->getTeam()) {
		if (selectedPiece->canMoveToPosition(position, pieces)) {
			moveSelectedPiece(position);
			unselectPiece();
			return true;
		}
	}
	return false;
}


void Board::moveSelectedPiece(sf::Vector2i targetPosition) {
	Piece* pieceAtThisPlace = pieces[targetPosition.x][targetPosition.y];
	sf::Vector2i previousPos = selectedPiece->getPosition();

	if (pieceAtThisPlace && pieceAtThisPlace->getTeam() == selectedPiece->getTeam() && selectedPiece->getType() == WizardPiece) {
		pieces[previousPos.x][previousPos.y] = pieceAtThisPlace;
		pieces[targetPosition.x][targetPosition.y] = selectedPiece;

		changedPiecesPositions.push_back(previousPos);
		changedPiecesPositions.push_back(targetPosition);
		selectedPiece->setPosition(targetPosition);
		pieceAtThisPlace->setPosition(previousPos);
		changedCases.push_back(previousPos);
		changedCases.push_back(targetPosition);
	}
	// roque
	else if (pieceAtThisPlace && pieceAtThisPlace->getTeam() == selectedPiece->getTeam()
		&& selectedPiece->getType() == KingPiece && pieceAtThisPlace->getType() == RookPiece
		) {
		if (targetPosition.x > previousPos.x) { // right
			pieces[previousPos.x][previousPos.y] = nullptr;
			pieces[previousPos.x + 1][previousPos.y] = pieceAtThisPlace;
			pieces[previousPos.x + 2][previousPos.y] = selectedPiece;
			pieces[previousPos.x + 3][previousPos.y] = nullptr;

			sf::Vector2i kingPos = sf::Vector2i(previousPos.x + 2, previousPos.y);
			sf::Vector2i rookPos = sf::Vector2i(previousPos.x + 1, previousPos.y);

			changedPiecesPositions.push_back(previousPos);
			changedPiecesPositions.push_back(targetPosition);
			changedPiecesPositions.push_back(rookPos);
			changedPiecesPositions.push_back(kingPos);
			selectedPiece->setPosition(kingPos);
			pieceAtThisPlace->setPosition(rookPos);
			changedCases.push_back(previousPos);
			changedCases.push_back(targetPosition);
			changedCases.push_back(rookPos);
			changedCases.push_back(kingPos);
		}
		else { // left
			pieces[previousPos.x][previousPos.y] = nullptr;
			pieces[previousPos.x - 1][previousPos.y] = pieceAtThisPlace;
			pieces[previousPos.x - 2][previousPos.y] = selectedPiece;
			pieces[previousPos.x - 4][previousPos.y] = nullptr;

			sf::Vector2i kingPos = sf::Vector2i(previousPos.x - 2, previousPos.y);
			sf::Vector2i rookPos = sf::Vector2i(previousPos.x - 1, previousPos.y);

			changedPiecesPositions.push_back(previousPos);
			changedPiecesPositions.push_back(targetPosition);
			changedPiecesPositions.push_back(rookPos);
			changedPiecesPositions.push_back(kingPos);
			selectedPiece->setPosition(kingPos);
			pieceAtThisPlace->setPosition(rookPos);
			changedCases.push_back(previousPos);
			changedCases.push_back(targetPosition);
			changedCases.push_back(rookPos);
			changedCases.push_back(kingPos);
		}
	}
	else if (!pieceAtThisPlace && selectedPiece->getType() == PawnPiece && abs(previousPos.x - targetPosition.x) == 1) {
		// mange en passant le pion
		sf::Vector2i pawnToEatPos = sf::Vector2i(targetPosition.x, previousPos.y);
		Piece* pawnToEat = pieces[pawnToEatPos.x][pawnToEatPos.y];
		if (pawnToEat) {
			std::vector<sf::Vector2i> reachablePositions = pawnToEat->getReachablePositions();
			for (unsigned int i = 0; i < reachablePositions.size(); ++i) {
				reachableCases[reachablePositions[i].x][reachablePositions[i].y].removePiece(pawnToEat);
			}
			delete pawnToEat;
			pieces[pawnToEatPos.x][pawnToEatPos.y] = nullptr;
		}
		pieces[targetPosition.x][targetPosition.y] = selectedPiece;
		pieces[previousPos.x][previousPos.y] = nullptr;

		changedPiecesPositions.push_back(previousPos);
		changedPiecesPositions.push_back(targetPosition);
		changedPiecesPositions.push_back(pawnToEatPos);
		changedCases.push_back(previousPos);
		changedCases.push_back(targetPosition);
		changedCases.push_back(pawnToEatPos);

		selectedPiece->setPosition(targetPosition);
	}
	else {
		if (pieceAtThisPlace) {
			std::vector<sf::Vector2i> reachablePositions = pieceAtThisPlace->getReachablePositions();
			for (unsigned int i = 0; i < reachablePositions.size(); ++i) {
				reachableCases[reachablePositions[i].x][reachablePositions[i].y].removePiece(pieceAtThisPlace);
			}
			delete pieceAtThisPlace;
		}

		// vérification d'activation du enPassant pour le prochain tour
		if (selectedPiece->getType() == PawnPiece && abs(previousPos.y - targetPosition.y) == 2) { 
			enPassantPossibleNextTurn = true;
			enPassantPiecePosition = targetPosition;
		}

		if (selectedPiece->getType() == PawnPiece && (targetPosition.y == 0 || targetPosition.y == height - 1)) {
			//promotion
			int promValue;

			Player* playingPlayer = player1->playing ? player1 : player2;
			if (playingPlayer->getControllerType() == Human) {
				std::cout << "1: Dame, 2: Cavalier, 3: Tour, 4: Fou" << std::endl;
				std::cin >> promValue;
			}
			else {
				promValue = 1;
			}

			Piece* previousPiece = pieces[previousPos.x][previousPos.y];
			switch (promValue) {
			case 2:
				pieces[previousPos.x][previousPos.y] = new Knight(selectedPiece->getTeam(), sf::Vector2i(previousPos.x, previousPos.y));
				break;
			case 3:
				pieces[previousPos.x][previousPos.y] = new Rook(selectedPiece->getTeam(), sf::Vector2i(previousPos.x, previousPos.y));
				break;
			case 4:
				pieces[previousPos.x][previousPos.y] = new Bishop(selectedPiece->getTeam(), sf::Vector2i(previousPos.x, previousPos.y));
				break;
			default:
				pieces[previousPos.x][previousPos.y] = new Queen(selectedPiece->getTeam(), sf::Vector2i(previousPos.x, previousPos.y));
				break;
			}
			std::vector<sf::Vector2i> reachablePositions = previousPiece->getReachablePositions();
			for (unsigned int i = 0; i < reachablePositions.size(); ++i) {
				reachableCases[reachablePositions[i].x][reachablePositions[i].y].replacePiece(previousPiece, pieces[previousPos.x][previousPos.y]);
			}
			delete previousPiece;
			selectedPiece = pieces[previousPos.x][previousPos.y];

			//waitingPromotion = true;
			//promotionPosition = previousPos;
		}

		pieces[targetPosition.x][targetPosition.y] = selectedPiece;
		pieces[previousPos.x][previousPos.y] = nullptr;

		changedPiecesPositions.push_back(previousPos);
		changedPiecesPositions.push_back(targetPosition);
		selectedPiece->setPosition(targetPosition);
		changedCases.push_back(previousPos);
		changedCases.push_back(targetPosition);
	}
}


void Board::updateAllReachablePositions() {
	Player* playingPlayer = player1->playing ? player1 : player2;

	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < height; ++y) {
			Piece* piece = pieces[x][y];
			if (piece && piece->getTeam() != playingPlayer->getTeam()) {
				udpateReachablePositions(piece);
			}
		}
	}

	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < height; ++y) {
			Piece* piece = pieces[x][y];
			if (piece && piece->getTeam() == playingPlayer->getTeam()) {
				udpateReachablePositions(piece);
			}
		}
	}

	enPassantPossibleNextTurn = false;
}

void Board::udpateReachablePositions(Piece* piece) {
	Player* playingPlayer = player1->playing ? player1 : player2;
	std::vector<sf::Vector2i> prevReachablePositions = piece->getReachablePositions();
	for (unsigned int i = 0; i < prevReachablePositions.size(); ++i) {
		reachableCases[prevReachablePositions[i].x][prevReachablePositions[i].y].removePiece(piece);
	}
	piece->updateReachablePositions(pieces, reachableCases, playingPlayer);
	if (enPassantPossibleNextTurn && piece->getType() == PawnPiece) {
		((Pawn*)piece)->checkEnPassant(pieces, reachableCases, playingPlayer, enPassantPiecePosition);
	}

	std::vector<sf::Vector2i> reachablePositions = piece->getReachablePositions();
	for (unsigned int i = 0; i < reachablePositions.size(); ++i) {
		reachableCases[reachablePositions[i].x][reachablePositions[i].y].addPiece(piece);
	}
}

bool Board::isKingMat(King* king) {
	Player* playingPlayer = player1->playing ? player1 : player2;


	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < height; ++y) {
			Piece* piece = pieces[x][y];
			if (piece && piece->getTeam() == playingPlayer->getTeam()) {
				if (piece->getReachablePositions().size() > 0) {
					return false;
				}
			}
		}
	}

	return true;
}


///////////////////////////////////////////////////////////////
//                           GET                             //
///////////////////////////////////////////////////////////////

CaseColor Board::getCaseColor(sf::Vector2i position) {
	Player* playingPlayer = player1->playing ? player1 : player2;

	CaseColor caseColor = Black;
	if (position.x % 2 == 1 && position.y % 2 == 0 || position.x % 2 == 0 && position.y % 2 == 1) {
		caseColor = White;
	}

	bool positionSelected = false;
	Piece* pieceAtPos = nullptr;
	if (selectedPiece && selectedPiece->getPosition() == position) {
		positionSelected = true;
		pieceAtPos = selectedPiece;
		
	}
	else if (positionUnderMouse == position) {
		positionSelected = true;
		pieceAtPos = pieces[position.x][position.y];
	}

	if (positionSelected) {
		if (caseColor == Black) {
			if (pieceAtPos) {
				if (playingPlayer->getTeam() == pieceAtPos->getTeam())
					caseColor = SelectedBlackBlue;
				else
					caseColor = SelectedBlackRed;
			}
			else caseColor = SelectedBlack;
		}
		else {
			if (pieceAtPos) {
				if (playingPlayer->getTeam() == pieceAtPos->getTeam())
					caseColor = SelectedWhiteBlue;
				else
					caseColor = SelectedWhiteRed;
			}
			else caseColor = SelectedWhite;
		}
	}

	return caseColor;
}

CaseColor Board::getCaseColor(int x, int y) {
	return getCaseColor(sf::Vector2i(x, y));
}


std::vector<sf::Vector2i>& Board::getChangedCases() {
	return changedCases;
}

int Board::getWidth() {
	return width;
}

int Board::getHeight() {
	return height;
}

Piece*** Board::getPieces() {
	return pieces;
}

bool Board::isInBoard(sf::Vector2i position) {
	return isInBoard(position.x, position.y);
}

bool Board::isInBoard(int x, int y) {
	return (x >= 0 && x < width && y >= 0 && y < height);
}

Player* Board::getPlayer1() {
	return player1;
}

Player* Board::getPlayer2() {
	return player2;
}

King* Board::getPlayer1King() {
	return player1King;
}

King* Board::getPlayer2King() {
	return player2King;
}

bool Board::isWaitingPromotion() {
	return waitingPromotion;
}

///////////////////////////////////////////////////////////////
//                           SET                             //
///////////////////////////////////////////////////////////////

void Board::selectPiece(sf::Vector2i position) {
	if (position.x < 0 || position.x >= width || position.y < 0 || position.y >= height) return;

	if (selectedPiece) {
		// movement etc
		if (selectedPiece->getPosition() != position) {
			changedCases.push_back(selectedPiece->getPosition());
			selectedPiece = pieces[position.x][position.y];
			hasChanged = true;
			changedCases.push_back(position);
		}
	}
	else {
		selectedPiece = pieces[position.x][position.y];
		hasChanged = true;
		changedCases.push_back(position);
	}
}

void Board::unselectPiece() {
	if (selectedPiece) {
		changedCases.push_back(selectedPiece->getPosition());
		selectedPiece = nullptr;
		hasChanged = true;
	}
}


void Board::setPositionUnderMouse(sf::Vector2i position) {
	if (position != positionUnderMouse) {
		if (positionUnderMouse.x != -1) {
			changedCases.push_back(positionUnderMouse);
		}
		positionUnderMouse = position;
		changedCases.push_back(position);
		hasChanged = true;
	}
}

Piece* Board::getSelectedPiece() {
	return selectedPiece;
}