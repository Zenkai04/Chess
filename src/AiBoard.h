#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class Move {
	public:
		Move() {}
		Move(sf::Vector2i from, sf::Vector2i to) {
			this->from = from; this->to = to;
		}
		sf::Vector2i from;
		sf::Vector2i to;
};

struct BestMove {
	int value;
	Move move;
};

class AiBoard
{
public:
	AiBoard(int width, int height);
	~AiBoard();

	void update(int** newBoard, int playerTeam);

	std::vector<Move> getPossibleMoves(int playerFactor);

	void move(Move movement);

	void undo();

	int evaluate();

	bool kingAlive(int playerFactor);

private:
	int playerTeam;

	void cleanBoard(int** board);

	std::vector<Move> getPieceMoves(sf::Vector2i position, int score);
	std::vector<Move> getPawnMoves(sf::Vector2i position, int score);
	std::vector<Move> getKnightMoves(sf::Vector2i position, int score);
	std::vector<Move> getBishopMoves(sf::Vector2i position, int score);
	std::vector<Move> getRookMoves(sf::Vector2i position, int score);
	std::vector<Move> getQueenMoves(sf::Vector2i position, int score);
	std::vector<Move> getKingMoves(sf::Vector2i position, int score);

	std::vector<Move> getMinerMoves(sf::Vector2i position, int score);
	std::vector<Move> getArcherMoves(sf::Vector2i position, int score);
	std::vector<Move> getWizardMoves(sf::Vector2i position, int score);


	bool isInMap(int x, int y, int width, int height);

	int** board = nullptr;

	std::vector<int**> previousStates;

	int width;
	int height;
};

