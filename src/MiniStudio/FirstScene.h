#pragma once
#include "Scene.h"
#include "TmxThing.h"
#include "Player.h"
#include "Wall.h"


class FirstScene : public Scene
{
private:
	TmxMap m_map;
	Player* m_player;


public:
	void OnInitialize() override;
	void OnWindowEvent(const sf::Event& event) override;
	void OnUpdate() override;
	void OnDraw(sf::RenderTarget& target) override;
};

