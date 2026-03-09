#include "InputManager.h"

#include "GameManager.h"
#include <SFML/Graphics.hpp>


void InputManager::update()
{

	//mise a jours des etats des inputs

	for (auto& paire : m_joystickStates) {
		if (paire.second.isDown) { paire.second.isDown = false; paire.second.isHeld = true; }
		if (paire.second.isRelease) { paire.second.isRelease = false; paire.second.isHeld = false; }
	}
	for (auto& paire : m_keysStates) {
		if (paire.second.isDown) { paire.second.isDown = false; paire.second.isHeld = true; }
		if (paire.second.isRelease) { paire.second.isRelease = false; paire.second.isHeld = false; }
	}
	for (auto& paire : m_mouseStates) {
		if (paire.second.isDown) { paire.second.isDown = false; paire.second.isHeld = true; }
		if (paire.second.isRelease) { paire.second.isRelease = false; paire.second.isHeld = false; }
	}



	sf::Event event;
	while (GameManager::Get()->mpWindow->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			GameManager::Get()->mpWindow->close();
			break;

		case sf::Event::Resized:
		case sf::Event::LostFocus:
		case sf::Event::GainedFocus:
		case sf::Event::MouseEntered:
		case sf::Event::MouseLeft:
			GameManager::Get()->mpScene->OnWindowEvent(event);
			break;


		case sf::Event::JoystickButtonPressed:
			m_joystickStates[event.joystickButton.button] = { true, false, false };
			break;

		case sf::Event::JoystickButtonReleased:
			m_joystickStates[event.joystickButton.button] = { false, false, true };
			break;

		case sf::Event::KeyPressed:
			m_keysStates[event.key.code] = { true, false, false };
			break;

		case sf::Event::KeyReleased:
			m_keysStates[event.key.code] = { false, false, true };
			break;

		case sf::Event::MouseButtonPressed:
			m_mouseStates[event.mouseButton.button] = { true, false, false };
			break;

		case sf::Event::MouseButtonReleased:
			m_mouseStates[event.mouseButton.button] = { false, false, true };
			break;

		case sf::Event::MouseMoved:
			m_mousePos = { event.mouseMove.x, event.mouseMove.y };
			break;


		default:
			//il reste pas mal d'event, shift click sur sf::Event pour les voirs
			break;
		}
	}


}

bool InputManager::isJoystickDown(const sf::Joystick::Axis _joystick) const
{
	if (auto it = m_joystickStates.find(_joystick); it != m_joystickStates.end())
		return it->second.isDown;
	return false;
}

bool InputManager::isJoystickHeld(const sf::Joystick::Axis _joystick) const
{
	if (auto it = m_joystickStates.find(_joystick); it != m_joystickStates.end())
		return it->second.isHeld;
	return false;
}

bool InputManager::isJoystickRelease(const sf::Joystick::Axis _joystick) const
{
	if (auto it = m_joystickStates.find(_joystick); it != m_joystickStates.end())
		return it->second.isRelease;
	return false;
}

bool InputManager::isKeyDown(const sf::Keyboard::Key _key) const
{
	if (auto it = m_keysStates.find(_key); it != m_keysStates.end())
		return it->second.isDown;
	return false;
}

bool InputManager::isKeyHeld(const sf::Keyboard::Key _key) const
{

	if (auto it = m_keysStates.find(_key); it != m_keysStates.end())
		return it->second.isHeld;
	return false;
}

bool InputManager::isKeyRelease(const sf::Keyboard::Key _key) const
{

	if (auto it = m_keysStates.find(_key); it != m_keysStates.end())
		return it->second.isRelease;
	return false;
}

bool InputManager::isMouseButtonDown(const sf::Mouse::Button _btn) const
{
	if (auto it = m_mouseStates.find(_btn); it != m_mouseStates.end())
		return it->second.isDown;
	return false;
}

bool InputManager::isMouseButtonHeld(const sf::Mouse::Button _btn) const
{
	if (auto it = m_mouseStates.find(_btn); it != m_mouseStates.end())
		return it->second.isHeld;
	return false;
}

bool InputManager::isMouseButtonRelease(const sf::Mouse::Button _btn) const
{
	if (auto it = m_mouseStates.find(_btn); it != m_mouseStates.end())
		return it->second.isRelease;
	return false;
}
