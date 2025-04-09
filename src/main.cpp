#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameManager.h"
#include "BoardType.h"

using namespace std;

int main() {
	cout << "Hello" << endl;

	int gameType;
	cout << "Choisir le mode de jeu" << endl;
	cout << "1: Standard, 2: Special" << endl;
	cin >> gameType;

	GameManager gameManager = GameManager((BoardType)gameType);
	gameManager.init();

    return 0;
}