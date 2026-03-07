#pragma once
#include "Scene.h"
#include "TmxThing.h"
#include "Player.h"
#include "RectPlayer.h"
#include "Wall.h"


class FirstScene : public Scene
{
private:
	Player* m_player;
	RectPlayer* m_rectPlayer;

public:
	void OnInitialize() override;
	void OnWindowEvent(const sf::Event& event) override;
	void OnUpdate() override;
	void OnDraw(sf::RenderTarget& target) override;
};

