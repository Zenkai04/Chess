#include "GameManager.h"
#include <stdlib.h>
#include <time.h>

GameManager::GameManager(BoardType boardType) {
    resourceManager = new ResourceManager();
    player1 = new Player(1, Human);
    player2 = new Player(2, AI);
    player1->setAiLevel(5);
    player2->setAiLevel(4);
    playingPlayer = player1;
    playingPlayer->playing = true;
    board = new Board(boardType, player1, player2);
    srand(time(NULL));
}

GameManager::~GameManager() {
    if (player1) {
        delete player1;
    }
    if (player2) delete player2;
    if (board) delete board;
    if (graphicManager) delete graphicManager;
    if (inputManager) delete inputManager;
    if (resourceManager) delete resourceManager;
}


////////////////////////////////////////////////////////////////
//                          PUBLIC                            //
////////////////////////////////////////////////////////////////

void GameManager::init() {
    gameLoop();
}



////////////////////////////////////////////////////////////////
//                          PRIVATE                           //
////////////////////////////////////////////////////////////////

void GameManager::gameLoop() {
    window = new  sf::RenderWindow(sf::VideoMode(900, 900), "CHESS 2.0");
    graphicManager = new GraphicManager(window, sf::Vector2i(900, 900), board);
    inputManager = new InputManager(graphicManager);

    while (window->isOpen())
    {
        inputManager->updateWindowEvents();

        if (inputManager->quit) {
            window->close();
            break;
        }

        if (!mat) {
            if (playingPlayer->getControllerType() == Human) {
                if (inputManager->leftClickUp) {
                    manageLeftClickRelease();
                }
                if (inputManager->rightClickUp) {
                    manageRightClickRelease();
                }
            }
            else {
                Move bestMove = playingPlayer->getBestMove(board->getPieces(), board->getWidth(), board->getHeight());
                board->selectPiece(bestMove.from);
                ++turnCount;
                if (!board->selectedPieceAction(bestMove.to)) {

                }
                endPlayerTurn();
            }
          
            updatePositionUnderMouse();

            graphicManager->updateRendering();

            if (mat) {
                Sleep(1000);
                graphicManager->renderMatPlayer(matPlayer);
            }
        }
    }
}

void GameManager::endPlayerTurn() {
    playingPlayer->playing = false;
    if (playingPlayer == player1) {
        playingPlayer = player2;
    }
    else {
        playingPlayer = player1;
    }
    playingPlayer->playing = true;

    board->updateAllReachablePositions();

    checkMat();
}

void GameManager::checkMat() {
    King* playerKing = nullptr;
    if (playingPlayer->getTeam() == 1) {
        playerKing = board->getPlayer1King();
    }
    else {
        playerKing = board->getPlayer2King();
    }

    if (board->isKingMat(playerKing)) {
        mat = true;
        matPlayer = playingPlayer;
        std::cout << "King Mat !!!!!" << playerKing->getTeam() << std::endl;
    }
}



void GameManager::manageLeftClickRelease() {
    sf::Vector2i position = inputManager->getMousePosition();

    if (isInBoard(position)) {
        sf::Vector2i boardPosition = getPositionInBoard(position);
        std::cout << "Board Pos" << boardPosition.x << " " << boardPosition.y << std::endl;
        board->selectPiece(boardPosition);
    }
}

void GameManager::manageRightClickRelease() {
    sf::Vector2i position = inputManager->getMousePosition();

    if (isInBoard(position)) {
        sf::Vector2i boardPosition = getPositionInBoard(position);
        if (board->selectedPieceAction(boardPosition)) {
            if(!board->isWaitingPromotion())
                endPlayerTurn();
        }
    }
}


bool GameManager::isInBoard(sf::Vector2i screenPosition) {
    int width = board->getWidth();
    int height = board->getHeight();

    return (
        screenPosition.x >= boardShiftPos.x && screenPosition.x < width * caseSize.x + boardShiftPos.x
        && screenPosition.y >= boardShiftPos.y && screenPosition.y < height * caseSize.y + boardShiftPos.y
    );
}

sf::Vector2i GameManager::getPositionInBoard(sf::Vector2i screenPosition) {
    int height = board->getHeight();

    int x = (screenPosition.x - (int)boardShiftPos.x) / (int)caseSize.x;
    int y = height - 1 - (screenPosition.y - (int)boardShiftPos.y) / (int)caseSize.y;
    return sf::Vector2i(x, y);
}


void GameManager::updatePositionUnderMouse() {
    sf::Vector2i mousePosition = inputManager->getMousePosition();
    if (isInBoard(mousePosition)) {
        sf::Vector2i boardPosition = getPositionInBoard(mousePosition);
        board->setPositionUnderMouse(getPositionInBoard(mousePosition));
    }
}
