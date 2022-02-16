#include "pch.h"
#include "CKeyManager.h"
#include "CCore.h"

int arrVK[(int)KEY::KEY_END] =
{
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',

	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,
	VK_F10,
	VK_F11,
	VK_F12,

	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'0',

	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	VK_SPACE,
	VK_TAB,
	VK_ESCAPE,
	VK_RETURN,
	VK_LSHIFT,
	VK_LCONTROL,
	VK_LMENU,

	VK_LBUTTON,
	VK_RBUTTON,
};

CKeyManager::CKeyManager()
	: m_bMouseFocus(true)
{
}

CKeyManager::~CKeyManager()
{
}

void CKeyManager::init()
{
	for (int i = 0; i < (int)KEY::KEY_END; ++i)
	{
		m_vecKey.push_back(tKey{ KEY_STATE::NONE, false });
	}
}

void CKeyManager::update()
{
	HWND hWnd = GetFocus();

	if (hWnd != nullptr)
	{
		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			if (GetAsyncKeyState(arrVK[i]) & 0x8000)
			{
				if (!m_vecKey[i].bPrevPress)
				{
					m_vecKey[i].eState = KEY_STATE::TAP;
					m_vecKey[i].bPrevPress = true;
				}
				else
				{
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
			}
			else
			{
				if (m_vecKey[i].bPrevPress)
				{
					m_vecKey[i].eState = KEY_STATE::AWAY;
					m_vecKey[i].bPrevPress = false;
				}
				else
				{
					m_vecKey[i].eState = KEY_STATE::NONE;
				}
			}
		}

		m_mousePrevPos = m_mousePos;

		POINT pos = {};
		GetCursorPos(&pos);
		ScreenToClient(CCore::GetInst()->GetWindowHandle(), &pos);
		m_mousePos = Vec2((float)pos.x, (float)pos.y);

		m_mouseDir = m_mousePos - m_mousePrevPos;
	}
}

bool CKeyManager::NotPressAnyKey()
{
	for (size_t i = 0; i < m_vecKey.size(); ++i)
	{
		if (KEY_STATE::NONE != m_vecKey[i].eState)
			return false;
	}

	return true;
}
