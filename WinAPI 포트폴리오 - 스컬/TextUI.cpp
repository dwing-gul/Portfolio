#include "stdafx.h"
#include "TextUI.h"
#include "Core.h"
#include "SceneManager.h"
#include "PathManager.h"
#include "Player.h"

TextUI::TextUI() :
	m_text(),
	m_subjectType(SUBJECT_TYPE::NONE),
	m_itemType(ITEM_TYPE::NONE),
	m_skulType(SKUL::NONE),
	m_bCurHP(false),
	m_bMaxHP(false),
	m_bBlack(false)
{
}

TextUI::~TextUI()
{
}

void TextUI::update()
{
	if (m_bCurHP)
	{
		Player* player = (Player*)SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);

		wchar_t curHP[10] = L"";
		_itow_s(player->GetCurHP(), curHP, 10);

		m_text = curHP;
	}
	else if (m_bMaxHP)
	{
		Player* player = (Player*)SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);

		wchar_t maxHP[10] = L"";
		_itow_s(player->GetMaxHP(), maxHP, 10);

		m_text = maxHP;
	}
}

void TextUI::render(HDC _dc)
{
	if (IsLook())
	{
		SetBkMode(_dc, TRANSPARENT);

		if(!m_bBlack)
			SetTextColor(_dc, RGB(255, 255, 255));
		else
			SetTextColor(_dc, RGB(0, 0, 0));

		SetTextAlign(_dc, TA_CENTER);

		TextOut(_dc, (int)GetFinalPos().x, (int)GetFinalPos().y, m_text.c_str(), (int)m_text.length());

		SetTextColor(_dc, RGB(0, 0, 0));
	}
}
