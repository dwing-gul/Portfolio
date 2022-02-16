#pragma once

class CEntity
{
private :
	static UINT g_nextID;
	wstring m_name;
	UINT m_ID;

public :
	void SetName(const wstring& _name) { m_name = _name; }
	wstring GetName() { return m_name; }
	UINT& GetID() { return m_ID; }

	virtual void UpdateData() {}

	void SaveToScene(FILE* _file);
	void LoadFromScene(FILE* _file);

	virtual CEntity* Clone() = 0;

public :
	CEntity();
	CEntity(const CEntity& _origin);
	virtual ~CEntity();
};

