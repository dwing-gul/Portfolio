#include "stdafx.h"
#include "AI.h"
#include "State.h"

AI::AI() :
	m_curState(nullptr),
	m_monster(nullptr)
{
}

AI::~AI()
{
	map<wstring, State*>::iterator iter = m_mapState.begin();
	for (; iter != m_mapState.end(); ++iter)
	{
		delete iter->second;
	}
}

void AI::update()
{
	if (nullptr != m_curState)
		m_curState->update();
}

void AI::lateUpdate()
{
	if (nullptr != m_curState)
		m_curState->lateUpdate();
}

void AI::AddState(const wstring& _key, State* _state)
{
	_state->m_ai = this;
	_state->SetName(_key);
	m_mapState.insert(make_pair(_key, _state));
}

void AI::ChangeState(const wstring& _key)
{
	if (nullptr != m_curState)
		m_curState->Exit();

	map<wstring, State*>::iterator iter = m_mapState.find(_key);
	assert(iter != m_mapState.end());

	m_curState = iter->second;

	m_curState->Enter();
}

wstring AI::GetCurStateName()
{
	map<wstring, State*>::iterator iter = m_mapState.begin();

	for (; iter != m_mapState.end(); ++iter)
	{
		if (iter->second == m_curState)
		{
			return iter->first;
		}
	}

	return 0;
}
