#pragma once
#include "imgui.h"

class GUI
{
private :
	string	m_name;
	bool	m_bActive;
	bool	m_bCancelButton;

public :
	void SetName(const string& _name) { m_name = _name; }
	const string& GetName() { return m_name; }

	virtual void Activate() { m_bActive = true; }
	void Deactivate() { m_bActive = false; }
	bool IsActive() { return m_bActive; }

	void SetCancelButton(bool _exist) { m_bCancelButton = _exist; }
	bool& IsCancelButton() { return m_bCancelButton; }

	virtual void init() {}
	virtual void update() = 0;
	
	static void GetResourceName(Res* _res, char* _out);

public :
	GUI();
	virtual ~GUI();
};

