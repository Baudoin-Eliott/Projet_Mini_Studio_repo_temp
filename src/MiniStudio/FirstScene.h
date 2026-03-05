#pragma once
#include "Scene.h"
#include "ActionMap.h"
#include "AudioMap.h"

enum class Action {Jump, Dash};
enum class Sounds {Boom};
enum class SoundGroup {explosion};



class FirstScene : public Scene
{
private:
	ActionMap<Action> m_actionMap;
	AudioMap<Sounds, SoundGroup> m_audioMap;

public:
	void OnInitialize() override;
	void OnWindowEvent(const sf::Event& event) override;
	void OnUpdate() override;
};

