#pragma once

class GameObject;

class EventManager :
	public Singleton<EventManager>
{
	SINGLE(EventManager)
private :
	vector<tEvent>		m_vectorEvent;
	vector<GameObject*> m_vectorDeadObj;
	bool				m_bEvent;
	bool				m_bOccur;

public :
	void update();
	void AddEvent(const tEvent& _event) { m_vectorEvent.push_back(_event); }
	void EventOccur() { m_bOccur = true; }
	bool IsHappend() { return m_bEvent; }

private :
	void Execute(const tEvent& _event);
};