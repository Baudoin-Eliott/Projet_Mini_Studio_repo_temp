#pragma once
#include "Scene.h"
class FirstScene : public Scene
{
public:
	void OnInitialize() override;
	void OnWindowEvent(const sf::Event& event) override;
	void OnUpdate() override;
};

