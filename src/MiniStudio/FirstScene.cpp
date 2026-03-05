#include "FirstScene.h"
#include "Debug.h"


void FirstScene::OnInitialize()
{
}

void FirstScene::OnEvent(const sf::Event& event)
{
}

void FirstScene::OnUpdate()
{
	Debug::DrawText(640, 200, "MiniStudio Project", 0.5, 0.5,sf::Color::White);
	Debug::DrawText(640, 260, "Groupe 9", 0.5, 0.5, sf::Color::White);
}
