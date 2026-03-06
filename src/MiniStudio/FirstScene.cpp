#include "FirstScene.h"
#include "Debug.h"

#include "InputManager.h"
#include "AudioManager.h"
#include <iostream>

void FirstScene::OnInitialize()
{
	m_player = CreateEntity<Player>(15, sf::Color::Red);
	m_player->init(150, 100);
	m_player->SetPosition(640, 360);
}

void FirstScene::OnWindowEvent(const sf::Event& event)
{
}

void FirstScene::OnUpdate()
{
}
