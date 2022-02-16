#include "stdafx.h"
#include "State.h"
#include "AI.h"
#include "EventManager.h"

State::State() :
    m_ai(nullptr),
    m_term(0.7f)
{
}

State::~State()
{
}

Monster* State::GetMonster()
{
    return m_ai->GetMonster();
}

void State::ChangeState(const wstring& _nextState)
{
    tEvent evn = {};
    evn.eType = EVENT_TYPE::CHANGE_STATE;
    evn.lParam = (DWORD_PTR)new wstring(_nextState);
    evn.wParam = (DWORD_PTR)m_ai;

    EventManager::GetInst()->AddEvent(evn);
}
