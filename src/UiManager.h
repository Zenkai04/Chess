#pragma once

#include "SFML/System/Vector2.hpp"

class UiManager
{
	class InstanceManager {
		UiManager* instance;
	public:
		InstanceManager() {
			instance = new UiManager();
		}
		~InstanceManager() {
			delete instance;
		}
		UiManager* getInstance() {
			return instance;
		}
	};

public:
	static UiManager* getInstance();
	static void renderUi();
	static void leftMouseReleased(sf::Vector2i screenPos);

private:
	UiManager();
	static InstanceManager instanceM;
};

