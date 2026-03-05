#include "FirstScene.h"
#include "Debug.h"

#include "InputManager.h"
#include "AudioManager.h"
#include <iostream>

void FirstScene::OnInitialize()
{
	m_actionMap.bind(Action::Jump, {{sf::Keyboard::Space, TriggerState::Held}});
	// donc la jump se declanche quand la touche space est maintenue

	m_actionMap.bind(Action::Dash, { {sf::Keyboard::Z, TriggerState::Held},{sf::Mouse::Left, TriggerState::Held} });
	// et dash, quand Z et click gauche en meme temps
	
	if (!m_audioMap.addSound(Sounds::Boom, { "res/PetitBoom.wav" })) {
		std::cout << "canot load sound Boom \n";
	}
}

void FirstScene::OnWindowEvent(const sf::Event& event)
{
}

void FirstScene::OnUpdate()
{
	Debug::DrawText(640, 200, "MiniStudio Project", 0.5, 0.5,sf::Color::White);
	Debug::DrawText(640, 230, "Groupe 9", 0.5, 0.5, sf::Color::White);

	if (m_actionMap.isActive(Action::Dash)) {
		Debug::DrawText(640, 280, "Dash !", 0.5, 0.5, sf::Color::White);
	}

	if (m_actionMap.isActive(Action::Jump)) {
		Debug::DrawText(640, 300, "Jump !", 0.5, 0.5, sf::Color::White);
	}

	if (InputManager::getInstance().isKeyDown(sf::Keyboard::Space)) {

		m_audioMap.playSound(Sounds::Boom);
	}

}
