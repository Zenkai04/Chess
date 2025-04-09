#include "GraphicManager.h"


GraphicManager::GraphicManager(sf::RenderWindow* window, sf::Vector2i windowSize, Board* board):
	window(window), windowSize(windowSize), board(board) {
	cases = nullptr;

	sf::Texture* selectionTexture = ResourceManager::getInstance()->getSelectionTexture();
	selectionSprite = new sf::Sprite(*selectionTexture);
	sf::Vector2u textureSize = selectionTexture->getSize();
	float xRatio = caseSize.x / textureSize.x * 1;
	float yRatio = caseSize.y / textureSize.y * 1;
	selectionSprite->setScale(xRatio, yRatio);
	loadFonts();
}

GraphicManager::~GraphicManager() {
	int width = sizeof(cases);
	if (cases) {
		for (unsigned int x = 0; x < width; ++x) {
			int height = sizeof(cases[x]);
			for (unsigned int y = 0; y < height; ++y) {
				if (cases[x][y]) {
					delete cases[x][y];
				}
			}
			delete cases[x];
		}

		delete cases;
	}

	if (selectionSprite) delete selectionSprite;
	for (unsigned int i = 0; i < reachablePositionSprites.size(); ++i) {
		delete reachablePositionSprites[i];
	}

	if (mainFont) delete mainFont;
}

void GraphicManager::loadFonts() {
	mainFont = new sf::Font();
	if (!mainFont->loadFromFile("assets/graphics/fonts/Montserrat-VariableFont_wght.ttf")) {
		std::cout << "Error" << std::endl;
	}
}



void GraphicManager::updateRendering() {
	if (!board || !board->hasChanged) return;

	window->clear(sf::Color(60, 40, 15));

	renderBoard();
	renderPieces();
	renderReachablePositions();

	renderUi();

	window->display();

	if(board) board->hasChanged = false;

}

void GraphicManager::renderBoard() {
	if (!cases) {
		initialiseBoard();
	}

	int width = board->getWidth();
	int height = board->getHeight();

	std::vector<sf::Vector2i> changedCases = board->changedCases;
	for (unsigned int i = 0; i < changedCases.size(); ++i) {
		CaseColor caseColor = board->getCaseColor(changedCases[i].x, changedCases[i].y);
		sf::Color color = getGraphicColor(caseColor);
		cases[changedCases[i].x][changedCases[i].y]->setFillColor(color);
	}
	board->changedCases.clear();

	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < height; ++y) {
			window->draw(*cases[x][y]);
		}
	}
}

void GraphicManager::initialiseBoard() {
	int width = board->getWidth();
	int height = board->getHeight();
	cases = new sf::RectangleShape **[width];
	for (unsigned int x = 0; x < width; ++x) {
		cases[x] = new sf::RectangleShape * [height];
		for (unsigned int y = 0; y < height; ++y) {
			CaseColor caseColor = board->getCaseColor(x, y);
			sf::Color color = getGraphicColor(caseColor);

			sf::RectangleShape* rect = new sf::RectangleShape(caseSize);
			rect->setFillColor(color);
			sf::Vector2f position = sf::Vector2f(x * caseSize.x, (height - y - 1) * caseSize.y) + boardShiftPos;
			rect->setPosition(position);
			cases[x][y] = rect;
		}
	}
}

sf::Color GraphicManager::getGraphicColor(CaseColor caseColor) {
	switch (caseColor)
	{
	case Black:
		return sf::Color(40, 30, 20);
	case White:
		return sf::Color(200, 200, 200);
	case SelectedBlack:
		return sf::Color(60, 50, 40);
	case SelectedBlackRed:
		return sf::Color(90, 50, 40);
	case SelectedBlackBlue:
		return sf::Color(60, 50, 90);
	case SelectedWhite:
		return sf::Color(220, 220, 220);
	case SelectedWhiteRed:
		return sf::Color(250, 180, 180);
	case SelectedWhiteBlue:
		return sf::Color(180, 180, 250);

	default:
		return sf::Color(20, 20, 20);
	}
}


void GraphicManager::renderPieces() {
	if (!piecesInitilized) {
		initialisePieces();
	}

	int width = board->getWidth();
	int height = board->getHeight();

	Piece* selectedPiece = board->getSelectedPiece();
	if (selectedPiece) {
		sf::Vector2i piecePos = selectedPiece->getPosition();
		sf::Vector2f position = sf::Vector2f(piecePos.x * caseSize.x, (height - piecePos.y - 1) * caseSize.y) + boardShiftPos;
		selectionSprite->setPosition(position);
		Player* playingPlayer = board->getPlayer1()->playing ? board->getPlayer1() : board->getPlayer2();
		if (playingPlayer->getTeam() == selectedPiece->getTeam()) {
			selectionSprite->setColor(sf::Color(20, 20, 255));
		}
		else {
			selectionSprite->setColor(sf::Color(255, 20, 20));
		}
		window->draw(*selectionSprite);
	}

	Piece*** pieces = board->getPieces();
	std::vector<sf::Vector2i> changedPiecesPositions = board->changedPiecesPositions;
	for (unsigned int i = 0; i < changedPiecesPositions.size(); ++i) {
		int x = changedPiecesPositions[i].x;
		int y = changedPiecesPositions[i].y;
		Piece* piece = pieces[x][y];
		if (piece == nullptr) continue;

		sf::Sprite* pieceSprite = piece->getSprite();
		sf::Vector2f position = sf::Vector2f(x * caseSize.x, (height - y - 1) * caseSize.y) + boardShiftPos;

		pieceSprite->setPosition(position);
	}
	changedPiecesPositions.clear();

	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < height; ++y) {
			Piece* piece = pieces[x][y];
			if (piece == nullptr) continue;

			sf::Sprite* pieceSprite = piece->getSprite();
			window->draw(*pieceSprite);
		}
	}
}

void GraphicManager::initialisePieces() {
	int width = board->getWidth();
	int height = board->getHeight();
	Piece*** pieces = board->getPieces();
	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < height; ++y) {
			Piece* piece = pieces[x][y];
			if (piece == nullptr) continue;

			sf::Sprite* pieceSprite = piece->getSprite();
			sf::Vector2f position = sf::Vector2f(x * caseSize.x, (height - y - 1) * caseSize.y) + boardShiftPos;

			pieceSprite->setPosition(position);
		}
	}
}


void GraphicManager::renderReachablePositions() {
	Piece* selectedPiece = board->getSelectedPiece();
	Player* playingPlayer = board->getPlayer1()->playing ? board->getPlayer1() : board->getPlayer2();
	if (selectedPiece && selectedPiece->getTeam() == playingPlayer->getTeam()) {
		int height = board->getHeight();
		std::vector<sf::Vector2i> reachablePositions = selectedPiece->getReachablePositions();

		for (unsigned int i = reachablePositionSprites.size(); i < reachablePositions.size(); ++i) {
			sf::Texture* texture = ResourceManager::getInstance()->getReachablePositionTexture();
			sf::Sprite* sprite = new sf::Sprite();
			sprite->setTexture(*texture);
			sprite->setColor(sf::Color(100, 150, 200, 200));
			reachablePositionSprites.push_back(sprite);
		}

		for (unsigned int i = 0; i < reachablePositions.size(); ++i) {
			sf::Sprite* sprite = reachablePositionSprites[i];
			sf::Vector2f position = sf::Vector2f(reachablePositions[i].x * caseSize.x, (height - reachablePositions[i].y - 1) * caseSize.y) + boardShiftPos;
			sprite->setPosition(position);
			window->draw(*sprite);
		}
	}
}


void GraphicManager::renderUi() {
	playingPlayerTxt.setFont(*mainFont);
	playingPlayerTxt.setCharacterSize(24);
	playingPlayerTxt.setFillColor(sf::Color::White);
	playingPlayerTxt.setStyle(sf::Text::Bold);

	std::string text = "Player ";
	if (board->getPlayer1()->playing) text += "1";
	else text += "2";

	playingPlayerTxt.setString(text);

	playingPlayerTxt.setOrigin(sf::Vector2f(playingPlayerTxt.getGlobalBounds().width, playingPlayerTxt.getGlobalBounds().height));
	playingPlayerTxt.setPosition(sf::Vector2f(800, 880));

	window->draw(playingPlayerTxt);
}


void GraphicManager::renderMatPlayer(Player* matPlayer) {
	matPlayerText.setFont(*mainFont);
	matPlayerText.setCharacterSize(80);
	matPlayerText.setFillColor(sf::Color::Black);
	matPlayerText.setStyle(sf::Text::Bold | sf::Text::Underlined);
	matPlayerText.setOutlineThickness(4);
	matPlayerText.setOutlineColor(sf::Color::White);

	matPlayerText2.setFont(*mainFont);
	matPlayerText2.setCharacterSize(80);
	matPlayerText2.setFillColor(sf::Color::Black);
	matPlayerText2.setStyle(sf::Text::Bold | sf::Text::Underlined);
	matPlayerText2.setOutlineThickness(4);
	matPlayerText2.setOutlineColor(sf::Color::White);

	std::string text = "Échec et mat !!!";
	matPlayerText.setString(text);

	std::string text2 = "Player ";
	if (board->getPlayer1()->playing) text2 += "1";
	else text2 += "2";
	text2 += " a perdu !";
	matPlayerText2.setString(text2);

	matPlayerText.setOrigin(sf::Vector2f(matPlayerText.getGlobalBounds().width/2, matPlayerText.getGlobalBounds().height / 2));
	matPlayerText.setPosition(sf::Vector2f(450, 400));

	matPlayerText2.setOrigin(sf::Vector2f(matPlayerText2.getGlobalBounds().width / 2, matPlayerText2.getGlobalBounds().height / 2));
	matPlayerText2.setPosition(sf::Vector2f(450, 500));

	window->clear(sf::Color(60, 40, 15));

	renderBoard();
	renderPieces();
	renderReachablePositions();

	renderUi();

	window->draw(matPlayerText);
	window->draw(matPlayerText2);

	window->display();
}


//////////////////////////////////////////////////////////////
//                           GET                            //
//////////////////////////////////////////////////////////////

sf::RenderWindow* GraphicManager::getWindow() {
	return window;
}