#pragma once
#include "Entity.h"
#include "ActionMap.h"

class Player : public Entity
{
private:
	enum class Action {
		MoveUp,
		MoveDown,
		MoveLeft,
		MoveRight,
	};

	int m_hp;
	int m_maxHp;


	ActionMap<Action> m_actions;

public:

	void init(float _speed, float _maxHp) { mSpeed = _speed; m_maxHp = _maxHp; m_hp = _maxHp; }

	void OnInitialize() override;

	void OnUpdate() override;


private:

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
};

