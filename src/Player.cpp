#include "Player.h"
#include <algorithm>
#include <random>
#include <stdlib.h>

Player::Player(int team, ControllerType controllerType) : team(team), controllerType(controllerType) {

}

Player::~Player() {

}

int Player::getTeam() {
	return team;
}

ControllerType Player::getControllerType() {
	return controllerType;
}

void Player::setAiLevel(int level) {
    aiLevel = level;
}

Move Player::getBestMove(Piece*** pieces, int width, int height) {
    AiBoard* game = new AiBoard(width, height);

    int** board = new int*[width];
    for (unsigned int x = 0; x < width; ++x) {
        board[x] = new int[height];
        for (unsigned int y = 0; y < height; ++y) {
            Piece* piece = pieces[x][y];
            if (piece) {
                int factor = piece->getTeam() == team ? 1 : -1;
                board[x][y] = piece->getScore() * factor;
            }
            else {
                board[x][y] = 0;
            }
        }
    }

    game->update(board, team);
    
    Move bestMove = calcBestMove(aiLevel, game).move;
    delete game;
    return bestMove;
}


BestMove Player::createBestMove(int value, Move move) {
    BestMove result = BestMove();
    result.value = value;
    result.move = move;
    return result;
}

BestMove Player::calcBestMove(int depth, AiBoard* game, int alpha, int beta, bool isMaximizingPlayer, bool firstLoop) {
    // Base case: evaluate board
    if (depth == 0) {
        int value = game->evaluate();
        return createBestMove(value, Move());
    }

    // Recursive case: search possible moves
    std::vector<Move> possibleMoves = game->getPossibleMoves(isMaximizingPlayer ? 1 : -1);
    if (firstLoop) {
        for (std::vector<Move>::iterator it = possibleMoves.begin(); it != possibleMoves.end();) {
            bool illegalMove = false;
            Move move = *it;
            // Make the move, but undo before exiting loop
            game->move(move);

            std::vector<Move> possibleMoves2 = game->getPossibleMoves((!isMaximizingPlayer) ? 1 : -1);

            for (unsigned int j = 0; j < possibleMoves2.size(); j++) {
                Move move2 = possibleMoves2[j];
                // Make the move, but undo before exiting loop
                game->move(move2);

                if (!game->kingAlive(isMaximizingPlayer ? 1 : -1)) {
                    illegalMove = true;
                    game->undo();
                    break;
                }
                
                game->undo();
            }

            game->undo();

            if (illegalMove) {
                it = possibleMoves.erase(it);
            }
            else {
                ++it;
            }
        }
    }


    if (possibleMoves.size() == 0) {
        return createBestMove(-10000, Move());
    }

    Move bestMove = possibleMoves[0]; // best move not set yet
    // Set random order for possible moves

    std::default_random_engine engine;
    engine.seed(rand());
    std::shuffle(std::begin(possibleMoves), std::end(possibleMoves), engine);
    // Set a default best move value
    int bestMoveValue = isMaximizingPlayer ? INT_MIN
        : INT_MAX;
    // Search through all possible moves
    for (unsigned int i = 0; i < possibleMoves.size(); i++) {
        Move move = possibleMoves[i];
        // Make the move, but undo before exiting loop
        game->move(move);
        // Recursively get the value from this move
        int value = calcBestMove(depth - 1, game, alpha, beta, !isMaximizingPlayer, false).value;

        if (isMaximizingPlayer) {
            // Look for moves that maximize position
            if (value > bestMoveValue) {
                bestMoveValue = value;
                bestMove = move;
            }
            alpha = alpha > value ? alpha : value;
        }
        else {
            // Look for moves that minimize position
            if (value < bestMoveValue) {
                bestMoveValue = value;
                bestMove = move;
            }
            beta = beta < value ? beta : value;
        }
        // Undo previous move
        game->undo();
        // Check for alpha beta pruning
        if (beta <= alpha) {
            break;
        }
    }
    return createBestMove(bestMoveValue, bestMove);
}