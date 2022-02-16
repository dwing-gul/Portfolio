#pragma once

class Monster;
class State;

class AI
{
private :
	map<wstring, State*> m_mapState;
	State* m_curState;
	Monster* m_monster;

public :
	Monster* GetMonster() { return m_monster; }
	State* GetCurState() { return m_curState; }
	wstring GetCurStateName();

	void update();
	void lateUpdate();

	void AddState(const wstring& _key, State* _state);
	void ChangeState(const wstring& _key);

	CLONE(AI)

public :
	AI();
	~AI();

	friend class Monster;
};

