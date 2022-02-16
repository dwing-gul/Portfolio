#include "stdafx.h"
#include "UI.h"
#include "KeyManager.h"
#include "Core.h"
#include "Tile.h"

UI::UI() :
	m_parentUI(nullptr),
	m_bLButtonDown(false),
	m_bRButtonDown(false),
	m_bMouseOn(false),
	m_bLook(true),
	m_uiType(UI_TYPE::GENERAL),
	m_brush(BRUSH_TYPE::WHITE),
	m_pen(PEN_TYPE::BLACK)
{
}

UI::~UI()
{
	for (size_t i = 0; i < m_vectorChildUI.size(); ++i)
	{
		delete m_vectorChildUI[i];
	}
}

UI::UI(const UI& _origin) :
	Obj(_origin),
	m_parentUI(nullptr),
	m_bLButtonDown(false),
	m_bRButtonDown(false),
	m_bMouseOn(false),
	m_bLook(_origin.m_bLook),
	m_uiType(_origin.m_uiType),
	m_brush(_origin.m_brush),
	m_pen(_origin.m_pen)
{
	for (size_t i = 0; i < _origin.m_vectorChildUI.size(); ++i)
	{
		AddChild(_origin.m_vectorChildUI[i]->Clone());
	}
}

void UI::update()
{
	if (!IsDead())
	{
		for (size_t i = 0; i < m_vectorChildUI.size(); ++i)
		{
			m_vectorChildUI[i]->update();
		}
	}
}

void UI::lateUpdate()
{
	if (!IsDead())
	{
		if (m_parentUI)
		{
			m_finalPos = GetPos() + m_parentUI->GetFinalPos();
		}
		else
		{
			m_finalPos = GetPos();
		}

		if (!m_bMouseOn)
		{
			m_bLButtonDown = false;
		}

		for (size_t i = 0; i < m_vectorChildUI.size(); ++i)
		{
			m_vectorChildUI[i]->lateUpdate();
		}
	}
}

void UI::render(HDC _dc)
{
	if (m_bLook)
	{
		if (nullptr == GetTexture())
		{
			HPEN redPen = Core::GetInst()->GetPen(PEN_TYPE::RED);
			HPEN greenPen = Core::GetInst()->GetPen(PEN_TYPE::GREEN);
			HBRUSH hollowBrush = Core::GetInst()->GetBrush(BRUSH_TYPE::HOLLOW);
			HPEN prevPen = nullptr;

			if (m_bMouseOn)
			{
				prevPen = (HPEN)SelectObject(_dc, redPen);
			}
			else
			{
				prevPen = (HPEN)SelectObject(_dc, greenPen);
			}

			HBRUSH prevBrush = (HBRUSH)SelectObject(_dc, hollowBrush);

			Rectangle(_dc, int(m_finalPos.x), int(m_finalPos.y), int(m_finalPos.x + GetScale().x), int(m_finalPos.y + GetScale().y));

			SelectObject(_dc, prevPen);
			SelectObject(_dc, prevBrush);
		}

		for (size_t i = 0; i < m_vectorChildUI.size(); ++i)
		{
			m_vectorChildUI[i]->render(_dc);
		}
	}
}

void UI::DeleteChildUI()
{
	for (size_t i = 0; i < m_vectorChildUI.size(); ++i)
	{
		delete m_vectorChildUI[i];
	}

	m_vectorChildUI.clear();
}

UI* UI::GetChildByName(const wstring& _name)
{
	for (size_t i = 0; i < m_vectorChildUI.size(); ++i)
	{
		if (m_vectorChildUI[i]->GetName() == _name)
			return m_vectorChildUI[i];
	}
	return nullptr;
}

bool UI::IsMouseOn(Vector2 _mousePos)
{
	Vector2 scale = GetScale();

	if (m_finalPos.x <= _mousePos.x && _mousePos.x <= m_finalPos.x + scale.x &&
		m_finalPos.y <= _mousePos.y && _mousePos.y <= m_finalPos.y + scale.y)
		return true;

	return false;
}

void UI::RButtonClick(Vector2 _mousePos)
{
	if (UI_TYPE::TILE == m_uiType)
	{
		Tile::g_imgIdx = -1;
	}
}