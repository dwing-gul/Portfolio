#pragma once

class Entity
{
private :
	static UINT	g_nextID;
	wstring		m_name;
	UINT		m_ID;

public :
	void SetName(const wstring& _name) { m_name = _name; }
	const wstring& GetName() { return m_name; }
	UINT GetID() { return m_ID; }

	virtual void UpdateData() {}

	void SaveToScene(FILE* _file);
	void LoadFromScene(FILE* _file);

	virtual Entity* Clone() = 0;

public :
	Entity();
	Entity(const Entity& _origin);
	virtual ~Entity();
};

