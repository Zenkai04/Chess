#include "ResourceManager.h"

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager::ResourceManager() {
	instance = this;

	selectionTexture = new sf::Texture();
	selectionTexture->loadFromFile(graphicsDir + "selection_sprite.png");

	reachablePositionTexture = new sf::Texture();
	reachablePositionTexture->loadFromFile(graphicsDir + "reachable_position.png");
	
	loadPieceTextures();
}

ResourceManager* ResourceManager::getInstance() {
	return instance;
}

ResourceManager::~ResourceManager() {
	if (w_king) delete w_king;
	if (b_king) delete b_king;
	if (w_queen) delete w_queen;
	if (b_queen) delete b_queen;
	if (w_rook) delete w_rook;
	if (b_rook) delete b_rook;
	if (w_knight) delete w_knight;
	if (b_knight) delete b_knight;
	if (w_bishop) delete w_bishop;
	if (b_bishop) delete b_bishop;
	if (w_pawn) delete w_pawn;
	if (b_pawn) delete b_pawn;

	if (w_wizard) delete w_wizard;
	if (b_wizard) delete b_wizard;
	if (w_miner) delete w_miner;
	if (b_miner) delete b_miner;
	if (w_archer) delete w_archer;
	if (b_archer) delete b_archer;



	if (selectionTexture) delete selectionTexture;
	if (reachablePositionTexture) delete reachablePositionTexture;
}

void ResourceManager::loadPieceTextures() {
	w_king = new sf::Texture();
	b_king = new sf::Texture();
	w_queen = new sf::Texture();
	b_queen = new sf::Texture();
	w_rook = new sf::Texture();
	b_rook = new sf::Texture();
	w_knight = new sf::Texture();
	b_knight = new sf::Texture();
	w_bishop = new sf::Texture();
	b_bishop = new sf::Texture();
	w_pawn = new sf::Texture();
	b_pawn = new sf::Texture();

	w_wizard = new sf::Texture();
	b_wizard = new sf::Texture();
	w_miner = new sf::Texture();
	b_miner = new sf::Texture();
	w_archer = new sf::Texture();
	b_archer = new sf::Texture();

	bool loadError = false;
	std::string piecesDir = graphicsDir + "pieces/";

	if (!w_king->loadFromFile(piecesDir + "w_king.png")) loadError = true;
	if (!b_king->loadFromFile(piecesDir + "b_king.png")) loadError = true;
	if (!w_queen->loadFromFile(piecesDir + "w_queen.png")) loadError = true;
	if (!b_queen->loadFromFile(piecesDir + "b_queen.png")) loadError = true;
	if (!w_rook->loadFromFile(piecesDir + "w_rook.png")) loadError = true;
	if (!b_rook->loadFromFile(piecesDir + "b_rook.png")) loadError = true;
	if (!w_knight->loadFromFile(piecesDir + "w_knight.png")) loadError = true;
	if (!b_knight->loadFromFile(piecesDir + "b_knight.png")) loadError = true;
	if (!w_bishop->loadFromFile(piecesDir + "w_bishop.png")) loadError = true;
	if (!b_bishop->loadFromFile(piecesDir + "b_bishop.png")) loadError = true;
	if (!w_pawn->loadFromFile(piecesDir + "w_pawn.png")) loadError = true;
	if (!b_pawn->loadFromFile(piecesDir + "b_pawn.png")) loadError = true;

	if (!w_wizard->loadFromFile(piecesDir + "w_wizard.png")) loadError = true;
	if (!b_wizard->loadFromFile(piecesDir + "b_wizard.png")) loadError = true;
	if (!w_miner->loadFromFile(piecesDir + "w_miner.png")) loadError = true;
	if (!b_miner->loadFromFile(piecesDir + "b_miner.png")) loadError = true;
	if (!w_archer->loadFromFile(piecesDir + "w_archer.png")) loadError = true;
	if (!b_archer->loadFromFile(piecesDir + "b_archer.png")) loadError = true;

	w_king->setSmooth(true);
	b_king->setSmooth(true);
	w_queen->setSmooth(true);
	b_queen->setSmooth(true);
	w_rook->setSmooth(true);
	b_rook->setSmooth(true);
	w_knight->setSmooth(true);
	b_knight->setSmooth(true);
	w_bishop->setSmooth(true);
	b_bishop->setSmooth(true);
	w_pawn->setSmooth(true);
	b_pawn->setSmooth(true);

	w_wizard->setSmooth(true);
	b_wizard->setSmooth(true);
	w_miner->setSmooth(true);
	b_miner->setSmooth(true);
	w_archer->setSmooth(true);
	b_archer->setSmooth(true);

	if (loadError) {
		std::cout << "Piece texture loading error" << std::endl;
	}
}


sf::Texture* ResourceManager::getPieceTexture(int team, PieceType pieceType) {
	switch (team) {
	case 1:
		switch (pieceType) {
		case KingPiece:
			return w_king;
			break;
		case QueenPiece:
			return w_queen;
			break;
		case RookPiece:
			return w_rook;
			break;
		case KnightPiece:
			return w_knight;
			break;
		case BishopPiece:
			return w_bishop;
			break;
		case PawnPiece:
			return w_pawn;
			break;
		case WizardPiece:
			return w_wizard;
			break;
		case MinerPiece:
			return w_miner;
			break;
		case ArcherPiece:
			return w_archer;
			break;
		}
		break;

	default:
		switch (pieceType) {
		case KingPiece:
			return b_king;
			break;
		case QueenPiece:
			return b_queen;
			break;
		case RookPiece:
			return b_rook;
			break;
		case KnightPiece:
			return b_knight;
			break;
		case BishopPiece:
			return b_bishop;
			break;
		case PawnPiece:
			return b_pawn;
			break;
		case WizardPiece:
			return b_wizard;
			break;
		case MinerPiece:
			return b_miner;
			break;
		case ArcherPiece:
			return b_archer;
			break;
		}
		break;
	}
}


sf::Texture* ResourceManager::getSelectionTexture() {
	return selectionTexture;
}

sf::Texture* ResourceManager::getReachablePositionTexture() {
	return reachablePositionTexture;
}


