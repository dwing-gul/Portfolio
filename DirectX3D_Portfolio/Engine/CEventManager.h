#pragma once

class CGameObject;

class CEventManager :
	public CSingleton<CEventManager>
{
	SINGLE(CEventManager);
private:
	vector<tEvent> m_vectorEvent;
	vector<tEvent> m_vecLaterEvent;
	vector<CGameObject*> m_vectorDeadObj;

	bool m_bEvent;
	bool m_bOccur;

public:
	void update();
	void AddEvent(const tEvent& _event) { m_vectorEvent.push_back(_event); }
	void AddEventLater(const tEvent& _event) { m_vecLaterEvent.push_back(_event); }
	void EventOccur() { m_bOccur = true; }
	bool IsHappened() { return m_bEvent; }

private:
	void Excute(const tEvent& _event);
};


