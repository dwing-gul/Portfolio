#include "stdafx.h"
#include "HollowBox.h"
#include "Core.h"
#include "SceneManager.h"
#include "ImageUI.h"
#include "KeyManager.h"

HollowBox::HollowBox() :
	m_imageUI(nullptr),
	m_finalLT(Vector2(0.f, 0.f)),
	m_bMove(true)
{
	
}

HollowBox::~HollowBox()
{
}

void HollowBox::update()
{
	if (!IsDead())
	{
		if (IsLButtonDown() && m_bMove)
		{
			SetPos(MOUSE_POS - m_diff);
		}

		if (nullptr != m_imageUI)
		{
			m_finalLT = m_imageUI->GetLT() + GetPos();
		}
	}
}

void HollowBox::lateUpdate()
{
	if (!IsDead())
	{
		Vector2 pos = GetPos();
		Vector2 scale = GetScale();

		if (nullptr != m_imageUI)
		{
			if (pos.x < 0)
				SetPos(Vector2(0.f, pos.y));
			if (pos.y < 0)
				SetPos(Vector2(pos.x, 0.f));
			if (pos.x + scale.x > m_imageUI->GetScale().x)
			{
				pos.x = m_imageUI->GetScale().x - scale.x;
				SetPos(Vector2(pos.x, pos.y));
			}
			if (pos.y + scale.y > m_imageUI->GetScale().y)
			{
				pos.y = m_imageUI->GetScale().y - scale.y;
				SetPos(Vector2(pos.x, pos.y));
			}

			if (scale.x > m_imageUI->GetScale().x)
				SetScale(Vector2(m_imageUI->GetScale().x, scale.y));
			if (scale.y > m_imageUI->GetScale().y)
				SetScale(Vector2(scale.x, m_imageUI->GetScale().y));
		}
	}
	UI::lateUpdate();
}

void HollowBox::render(HDC _dc)
{
	HPEN pen = Core::GetInst()->GetPen(PEN_TYPE::GREEN);
	HBRUSH brush = Core::GetInst()->GetBrush(BRUSH_TYPE::HOLLOW);

	HPEN prevPen = (HPEN)SelectObject(_dc, pen);
	HBRUSH prevBrush = (HBRUSH)SelectObject(_dc, brush);

	Rectangle(_dc, int(GetFinalPos().x), int(GetFinalPos().y), int(GetFinalPos().x + GetScale().x), int(GetFinalPos().y + GetScale().y));

	SelectObject(_dc, prevBrush);
	SelectObject(_dc, prevPen);

	const vector<UI*>& vectorChildUI = GetChild();
	for (size_t i = 0; i < vectorChildUI.size(); ++i)
	{
		vectorChildUI[i]->render(_dc);
	}
}

void HollowBox::LButtonDown(Vector2 _mousePos)
{
	UI::LButtonDown(_mousePos);

	m_diff = GetLButtonDownPos() - GetPos();
}

