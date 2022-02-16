#pragma once
#include "Monster.h"

class AI;

class State
{
private :
	wstring m_name;
	AI* m_ai;

public :
	void SetName(const wstring& _name) { m_name = _name; }
	const wstring& GetName() { return m_name; }
	AI* GetAI() { return m_ai; }
	Monster* GetMonster();

protected :
	void ChangeState(const wstring& _nextState);
	float m_term; // 애니메이션 전환에 일정한 텀을 줌

public :
	virtual void update() = 0;
	virtual void lateUpdate() {}
	virtual void Enter() = 0;
	virtual void Exit() = 0;

public :
	State();
	virtual ~State();

	friend class AI;
};

