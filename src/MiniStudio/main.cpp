#include <SFML/Graphics.hpp>
#include <iostream>


#include "GameManager.h"
#include "FirstScene.h"


#include <cstdlib>
#include <crtdbg.h>


int main()
{
	GameManager* pInstance = GameManager::Get();

	pInstance->CreateWindow(1280, 720, "FirstScene", 60, sf::Color::Black);

	pInstance->LaunchScene<FirstScene>();

	return 0;
}
