#include "FirstScene.h"
#include "Debug.h"
#include "TmxThing.h"

#include "InputManager.h"
#include "AudioManager.h"
#include <iostream>

void FirstScene::OnInitialize()
{
	m_map.load("res/base.tmx");

	for (const TmxObjectGroup& group : m_map.objectGroup)
	{
		if (group.name != "Collision")
			continue;

		for (const TmxObject& obj : group.objects)
		{
			Wall* wall = CreateEntity<Wall>(sf::Vector2f(obj.width, obj.height), sf::Color::Transparent);
			wall->SetPosition(obj.x, obj.y, 0.f, 0.f);
			wall->SetRigidBody(true);
			wall->SetStatic(true);
		}
	}

	m_player = CreateEntity<Player>(35, sf::Color::Red);
	m_player->init(150, 100);
	m_player->SetPosition(640, 360);
	m_player->SetRigidBody(true);

	m_rectPlayer = CreateEntity<RectPlayer>({75, 50}, sf::Color::Blue);
	m_rectPlayer->init(150, 100);
	m_rectPlayer->SetPosition(760, 360);
	m_rectPlayer->SetRigidBody(true);
	m_rectPlayer->SetStatic(true);

	m_cam->setPosition(m_player->GetPosition());
	GameManager::Get()->setCamera(*m_cam);
}

void FirstScene::OnWindowEvent(const sf::Event& event)
{
}

void FirstScene::OnDraw(sf::RenderTarget& target)
{
	target.draw(m_map);
}

void FirstScene::OnUpdate()
{
	Debug::DrawText(640, 200, "MiniStudio Project", 0.5, 0.5, sf::Color::White);
	Debug::DrawText(640, 230, "Groupe 9", 0.5, 0.5, sf::Color::White);

	m_cam->setPosition(m_player->GetPosition());
	GameManager::Get()->setCamera(*m_cam);
}
