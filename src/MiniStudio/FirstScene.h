#pragma once
#include "Scene.h"
#include "ActionMap.h"


enum class Action {Jump, Dash};




class FirstScene : public Scene
{
private:
	ActionMap<Action> m_actionMap;


public:
	void OnInitialize() override;
	void OnWindowEvent(const sf::Event& event) override;
	void OnUpdate() override;
};

