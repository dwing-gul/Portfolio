#pragma once

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	CHANGE_SCENE,
	SELECT_TILE,
	CHANGE_STATE,
	CHANGE_ANIM,
};

struct tEvent
{
	EVENT_TYPE eType;
	DWORD_PTR lParam;
	DWORD_PTR wParam;
};

class Obj;

class EventManager
{
	SINGLE(EventManager)
private :
	vector<tEvent> m_vectorEvent;
	vector<Obj*> m_vectorDeleteObj;

public :
	void AddEvent(const tEvent& _event) { m_vectorEvent.push_back(_event); }

	void update();

private :
	void Execute(const tEvent& _event);
};

