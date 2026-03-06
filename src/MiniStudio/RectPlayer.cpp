#include "RectPlayer.h"
#include "AssetsManager.h"

void RectPlayer::OnInitialize()
{

	m_actions.bind(Action::MoveUp, { {sf::Keyboard::Up, TriggerState::Held} });

	m_actions.bind(Action::MoveDown, { {sf::Keyboard::Down, TriggerState::Held} });


	m_actions.bind(Action::MoveLeft, { {sf::Keyboard::Left, TriggerState::Held} });

	m_actions.bind(Action::MoveRight, { {sf::Keyboard::Right, TriggerState::Held} });

}

void RectPlayer::OnUpdate()
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

void RectPlayer::MoveUp()
{
	SetDirection(0, -1);
}

void RectPlayer::MoveDown()
{
	SetDirection(0, 1);
}

void RectPlayer::MoveLeft()
{
	SetDirection(-1, 0);
}

void RectPlayer::MoveRight()
{
	SetDirection(1, 0);
}
