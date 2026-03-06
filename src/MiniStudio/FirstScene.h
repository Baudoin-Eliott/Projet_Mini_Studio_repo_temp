#pragma once
#include "Scene.h"
<<<<<<< Updated upstream
#include "ActionMap.h"
#include "AudioMap.h"

enum class Action {Jump, Dash};
enum class Sounds {Boom};
enum class SoundGroup {explosion};

=======
#include "Player.h"
#include "RectPlayer.h"
>>>>>>> Stashed changes


class FirstScene : public Scene
{
private:
<<<<<<< Updated upstream
	ActionMap<Action> m_actionMap;
	AudioMap<Sounds, SoundGroup> m_audioMap;
=======
	Player* m_player;
	RectPlayer* m_rectPlayer;
>>>>>>> Stashed changes

public:
	void OnInitialize() override;
	void OnWindowEvent(const sf::Event& event) override;
	void OnUpdate() override;
};

