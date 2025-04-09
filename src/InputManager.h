#pragma once

#include <SFML/Window.hpp>
#include "GraphicManager.h"

class InputManager
{
public:
	InputManager(GraphicManager* graphicManager);

	sf::Vector2i getMousePosition();
	void updateWindowEvents();

	bool quit = false;

	bool leftClickUp = false;
	bool rightClickUp = false;

private:
	GraphicManager* graphicManager;
};

