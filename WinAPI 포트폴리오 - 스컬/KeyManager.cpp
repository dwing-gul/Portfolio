#include "stdafx.h"
#include "KeyManager.h"
#include "Core.h"

int arrVK[(int)KEY::END] =
{
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
	'Z',
	'X',
	'C',
	'W',
	'A',
	'S',
	'D',
	'F',
	'L',
	'1',
	'2',
	'3',
	VK_F1,
	VK_F2,
	VK_SPACE,
	VK_TAB,
	VK_ESCAPE,
	VK_LBUTTON,
	VK_RBUTTON,
};

KeyManager::KeyManager()
{
}

KeyManager::~KeyManager()
{
}

void KeyManager::init()
{
	for (int i = 0; i < (int)KEY::END; ++i)
	{
		m_vectorKey.push_back(tKey{ KEY_STATE::NONE, false });
	}
}

void KeyManager::update()
{
	HWND hWnd = GetFocus();

	if (hWnd != nullptr)
	{
		for (int i = 0; i < (int)KEY::END; ++i)
		{
			if (GetAsyncKeyState(arrVK[i]) & 0x8000)
			{
				if (!m_vectorKey[i].bPrevPress)
				{
					m_vectorKey[i].eState = KEY_STATE::TAP;
					m_vectorKey[i].bPrevPress = true;
				}
				else
				{
					m_vectorKey[i].eState = KEY_STATE::HOLD;
				}
			}
			else
			{
				if (m_vectorKey[i].bPrevPress)
				{
					m_vectorKey[i].eState = KEY_STATE::AWAY;
					m_vectorKey[i].bPrevPress = false;
				}
				else
				{
					m_vectorKey[i].eState = KEY_STATE::NONE;
				}
			}
		}

		POINT pos = {};
		GetCursorPos(&pos);
		ScreenToClient(Core::GetInst()->GetWindowHandle(), &pos);
		m_mousePos = pos;
	}
}

bool KeyManager::NotPressAnyKey()
{
	for (size_t i = 0; i < m_vectorKey.size(); ++i)
	{
		if (KEY_STATE::NONE != m_vectorKey[i].eState)
			return false;
	}

	return true;
}
