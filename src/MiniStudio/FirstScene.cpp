#include "FirstScene.h"
#include "Debug.h"

#include "InputManager.h"


void FirstScene::OnInitialize()
{
}

void FirstScene::OnWindowEvent(const sf::Event& event)
{
}

void FirstScene::OnUpdate()
{
	Debug::DrawText(640, 200, "MiniStudio Project", 0.5, 0.5,sf::Color::White);
	Debug::DrawText(640, 260, "Groupe 9", 0.5, 0.5, sf::Color::White);

	if (InputManager::getInstance().isKeyHeld(sf::Keyboard::Space)) {
		Debug::DrawText(640, 320, "InputManager Marche", 0.5, 0.5, sf::Color::White);

	}


}
