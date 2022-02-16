#include "pch.h"
#include "CFontUIScript.h"
#include "CPlayerScript.h"
#include "CMonsterScript.h"
#include <Engine/CFontManager.h>
#include <Engine/CRenderManager.h>
#include <Engine/CCamera.h>
#include <Engine/CCore.h>

CFontUIScript::CFontUIScript() :
	CUIScript((UINT)SCRIPT_TYPE::FONTUISCRIPT),
	m_fontType(FONT_TYPE::GENERAL),
	m_fontSize(1.f),
	m_redColor(255),
	m_greenColor(255),
	m_blueColor(255),
	m_alphaColor(255),
	m_numberText(0),
	m_textPos(Vec2(0.f, 0.f)),
	m_bShow(true),
	m_bNum(false),
	m_bTime(false),
	m_cam(nullptr)
{
	m_fontColor = FONT_RGBA(m_redColor, m_greenColor, m_blueColor, m_alphaColor);
}

CFontUIScript::CFontUIScript(const CFontUIScript& _origin) :
	CUIScript(_origin),
	m_fontType(_origin.m_fontType),
	m_fontSize(_origin.m_fontSize),
	m_redColor(_origin.m_redColor),
	m_greenColor(_origin.m_greenColor),
	m_blueColor(_origin.m_blueColor),
	m_alphaColor(_origin.m_alphaColor),
	m_numberText(_origin.m_numberText),
	m_text(_origin.m_text),
	m_textPos(_origin.m_textPos),
	m_bShow(true),
	m_bNum(_origin.m_bNum),
	m_bTime(_origin.m_bTime),
	m_cam(nullptr)
{
	m_fontColor = FONT_RGBA(m_redColor, m_greenColor, m_blueColor, m_alphaColor);
}

CFontUIScript::~CFontUIScript()
{
}

void CFontUIScript::start()
{
	CUIScript::start();

	if (UI_TYPE::FONT_CHANGEABLE == m_uiType)
	{
	}
	else if (UI_TYPE::FONT_FIX == m_uiType)
	{
		if (m_bNum)
		{
			m_bNum = false;
			wchar_t num[255];
			swprintf_s(num, L"%d", m_numberText);
			m_text = num;
		}
	}

	Object()->SetFont(true);

	if (FONT_TYPE::TRACKING_LEVEL == m_fontType)
	{
		Object()->SetFont(false);
		MeshRender()->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(L"NumberMaterial")->Clone());
	}

	m_cam = CSceneManager::GetInst()->FindObjByName(L"MainCamera");
}

void CFontUIScript::update()
{
	CUIScript::update();

	if (UI_TYPE::FONT_CHANGEABLE == m_uiType)
	{
		if (m_bTime)
		{
			int min = m_numberText / 60;
			int sec = m_numberText % 60;

			if (m_bNum)
			{
				m_bNum = false;
				wchar_t num[255];
				swprintf_s(num, L"%02d : %02d", min, sec);
				m_text = num;
			}
		}
		else
		{
			if (m_bNum)
			{
				m_bNum = false;
				wchar_t num[255];
				swprintf_s(num, L"%d", m_numberText);
				m_text = num;
			}
		}

		if (FONT_TYPE::TRACKING_LEVEL == m_fontType)
		{
			const vector<CGameObject*>& vecChild = Object()->GetParent()->GetChilds();

			for (size_t i = 0; i < vecChild.size(); ++i)
			{
				if (vecChild[i]->GetName() == L"LevelFontPos")
				{
					if (nullptr == m_cam)
						break;

					Vec3 pos = vecChild[i]->Transform()->GetLocalPos();

					if (Object()->GetParent()->GetName() == L"RootTrackingUI")
						pos.x -= 5.f;
					else
						pos.x += 5.f;
					pos.z -= 1.f;

					Transform()->SetLocalPos(pos);
					break;
				}
			}

			m_text.clear();

			int level = 1;
			int numLength = 1;

			if (nullptr != m_target && m_target->GetObjectType() == OBJECT_TYPE::PLAYER)
			{
				level = ((CPlayerScript*)m_target->GetScripts()[0])->GetLevel();
			}
			else if (nullptr != m_target && m_target->GetObjectType() == OBJECT_TYPE::MONSTER)
			{
				level = ((CMonsterScript*)m_target->GetScripts()[0])->GetLevel();
			}

			if (level > 9)
			{
				numLength = 2;
			}

			if (Object()->GetParent()->GetName() == L"RootTrackingUI")
				Transform()->SetLocalScale(Vec3(25.f, 25.f, 1.f));
			else
				Transform()->SetLocalScale(Vec3(20.f, 20.f, 1.f));

			Ptr<CMaterial> material = MeshRender()->GetSharedMaterial();
			material->SetData(SHADER_PARAM::INT_0, &numLength);
			material->SetData(SHADER_PARAM::INT_1, &level);
		}
	}
	else if (UI_TYPE::FONT_FIX == m_uiType)
	{
	
	}
}

void CFontUIScript::lateUpdate()
{
	CUIScript::lateUpdate();
}

void CFontUIScript::render()
{
	if (m_bShow && !m_text.empty())
		CFontManager::GetInst()->DrawFont(m_text.c_str(), m_textPos.x, m_textPos.y, m_fontSize, m_fontColor);
}

void CFontUIScript::SaveToScene(FILE* _file)
{
	CUIScript::SaveToScene(_file);
	SaveWStringToFile(m_text, _file);
	SaveToFile(&m_fontType, _file);
	SaveToFile(&m_numberText, _file);
	SaveToFile(&m_textPos, _file);
	SaveToFile(&m_fontSize, _file);
	SaveToFile(&m_fontColor, _file);
	SaveToFile(&m_bNum, _file);
	SaveToFile(&m_bTime, _file);
}

void CFontUIScript::LoadFromScene(FILE* _file)
{
	CUIScript::LoadFromScene(_file);
	LoadWStringFromFile(m_text, _file);
	LoadFromFile(&m_fontType, _file);
	LoadFromFile(&m_numberText, _file);
	LoadFromFile(&m_textPos, _file);
	LoadFromFile(&m_fontSize, _file);
	LoadFromFile(&m_fontColor, _file);
	LoadFromFile(&m_bNum, _file);
	LoadFromFile(&m_bTime, _file);
}
