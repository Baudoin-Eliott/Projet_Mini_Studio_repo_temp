#pragma once
#include "Scene.h"
#include "Player.h"
#include "RectPlayer.h"


class FirstScene : public Scene
{
private:
	Player* m_player;
	RectPlayer* m_rectPlayer;

public:
	void OnInitialize() override;
	void OnWindowEvent(const sf::Event& event) override;
	void OnUpdate() override;
};

