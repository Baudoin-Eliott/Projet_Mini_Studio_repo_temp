#pragma once
#include "Scene.h"
#include "Player.h"


class FirstScene : public Scene
{
private:
	Player* m_player;

public:
	void OnInitialize() override;
	void OnWindowEvent(const sf::Event& event) override;
	void OnUpdate() override;
};

