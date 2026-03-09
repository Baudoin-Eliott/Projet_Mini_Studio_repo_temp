#include "Player.h"
#include "AssetsManager.h"

void Player::OnInitialize()
{
	m_actions.bind(Action::MoveUp, { {sf::Keyboard::Z, TriggerState::Held} });
	m_actions.bind(Action::MoveUp, { {sf::Joystick::isButtonPressed(0, JoystickButton::Triangle), TriggerState::Held}});

	m_actions.bind(Action::MoveDown, { {sf::Keyboard::S, TriggerState::Held} });

	m_actions.bind(Action::MoveLeft, { {sf::Keyboard::Q, TriggerState::Held} });

	m_actions.bind(Action::MoveRight, { {sf::Keyboard::D, TriggerState::Held} });


}

void Player::OnUpdate()
{
	//check et apply des mouvements
	if (m_actions.isActive(Action::MoveUp))
		MoveUp();

	if (m_actions.isActive(Action::MoveDown))
		MoveDown();

	if (m_actions.isActive(Action::MoveLeft))
		MoveLeft();

	if (m_actions.isActive(Action::MoveRight))
		MoveRight();


}

void Player::MoveUp()
{
	SetDirection(0, -1);
}

void Player::MoveDown()
{
	SetDirection(0, 1);
}

void Player::MoveLeft()
{
	SetDirection(-1, 0);
}

void Player::MoveRight()
{
	SetDirection(1, 0);
}
