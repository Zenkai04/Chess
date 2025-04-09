#include "InputManager.h"


InputManager::InputManager(GraphicManager* graphicManager) : graphicManager(graphicManager) {

}

sf::Vector2i InputManager::getMousePosition() {
	sf::RenderWindow* window = graphicManager->getWindow();
	if (!window) return sf::Vector2i(0, 0);
	return sf::Mouse::getPosition(*window);
}

void InputManager::updateWindowEvents() {
    leftClickUp = false;
    rightClickUp = false;
    quit = false;

    sf::RenderWindow* window = graphicManager->getWindow();
    if (!window) return;
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            quit = true;
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                leftClickUp = true;
            }
            if (event.mouseButton.button == sf::Mouse::Right) {
                rightClickUp = true;
            }
        }
    }
}
