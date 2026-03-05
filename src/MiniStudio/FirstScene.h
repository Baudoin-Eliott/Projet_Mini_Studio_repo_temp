#pragma once
#include "Scene.h"
class FirstScene : public Scene
{
public:
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
};

