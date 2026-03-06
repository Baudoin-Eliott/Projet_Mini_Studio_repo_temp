#include "FirstScene.h"
#include "Debug.h"

#include "InputManager.h"
#include "AudioManager.h"
#include <iostream>

void FirstScene::OnInitialize()
{
	m_player = CreateEntity<Player>(35, sf::Color::Red);
	m_player->init(150, 100);
	m_player->SetPosition(640, 360);
	m_player->SetRigidBody(true);

	m_rectPlayer = CreateEntity<RectPlayer>({75, 50}, sf::Color::Blue);
	m_rectPlayer->init(150, 100);
	m_rectPlayer->SetPosition(760, 360);
	m_rectPlayer->SetRigidBody(true);
	m_rectPlayer->SetStatic(true);
}

void FirstScene::OnWindowEvent(const sf::Event& event)
{
}

void FirstScene::OnUpdate()
{
	Debug::DrawText(640, 200, "MiniStudio Project", 0.5, 0.5, sf::Color::White);
	Debug::DrawText(640, 230, "Groupe 9", 0.5, 0.5, sf::Color::White);
}
