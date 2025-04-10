# Chess

Chess est une implémentation personnalisée du jeu d'échecs, avec des fonctionnalités supplémentaires telles que des pièces spéciales (Archer, Miner, Wizard) et des modes de jeu personnalisés.

## Fonctionnalités

- **Modes de jeu** :
  - Mode standard : Jeu d'échecs classique.
  - Mode spécial : Ajout de pièces spéciales (Archer, Miner, Wizard).
  
- **IA intégrée** :
  - Jouez contre une IA avec différents niveaux de difficulté.
  
- **Interface graphique** :
  - Utilisation de la bibliothèque SFML pour une interface graphique interactive.
  
- **Gestion des pièces** :
  - Chaque pièce a ses propres mouvements et comportements personnalisés.

```markdown
## Structure du projet

Voici la structure du projet :


Chess/
├── src/
│   ├── AiBoard.cpp
│   ├── Archer.cpp
│   ├── Bishop.cpp
│   ├── Board.cpp
│   ├── GameManager.cpp
│   ├── GraphicManager.cpp
│   ├── InputManager.cpp
│   ├── King.cpp
│   ├── Knight.cpp
│   ├── Miner.cpp
│   ├── Pawn.cpp
│   ├── Piece.cpp
│   ├── Player.cpp
│   ├── Queen.cpp
│   ├── ReachableCase.cpp
│   ├── ResourceManager.cpp
│   ├── Rook.cpp
│   ├── UiManager.cpp
│   ├── Wizard.cpp
│   └── main.cpp
├── ChessWanotech/
│   └── test.cpp
├── assets/
│   ├── graphics/
│   │   ├── pieces/
│   │   └── fonts/
├── captures/
└── README.md

```

## Prérequis

- **SFML 2.5.1** : La bibliothèque SFML est utilisée pour la gestion graphique, audio et des événements.
- **Compilateur C++** : Compatible avec C++11 ou supérieur.

## Installation

1. Clonez le dépôt :
   ```bash
   git clone https://github.com/votre-utilisateur/chess.git
   cd chess
   ```
2. Assurez-vous que SFML est installé et configuré sur votre système.

3. Compilez le projet avec votre IDE ou en ligne de commande :
   ```bash
   g++ -std=c++11 -I SFML-2.5.1/include -L SFML-2.5.1/lib src/*.cpp -o Chess -lsfml-graphics -lsfml-window -lsfml-system
   ```

4. Exécutez le programme :
   ```bash
   ./Chess
   ```

5. Utilisation
    1. Lancez le programme.
    2. Choisissez le mode de jeu :
        1 pour le mode standard.
        2 pour le mode spécial.
    3. Jouez contre un autre joueur ou contre l'IA.
## Licence

Ce projet est sous licence MIT. Consultez le fichier LICENSE pour plus d'informations.