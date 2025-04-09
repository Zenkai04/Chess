#include "UiManager.h"

UiManager::InstanceManager UiManager::instanceM;

UiManager::UiManager() {}

UiManager* UiManager::getInstance() {
	return instanceM.getInstance();
}