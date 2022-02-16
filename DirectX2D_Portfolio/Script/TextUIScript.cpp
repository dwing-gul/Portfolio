#include "pch.h"
#include "TextUIScript.h"
#include "CameraScript.h"
#include <Engine/FontManager.h>
#include <Engine/SceneManager.h>
#include <Engine/Transform.h>
#include <Engine/GameObject.h>

TextUIScript::TextUIScript() :
	UIScript(SCRIPT_TYPE::TEXTUISCRIPT),
	m_target(nullptr),
	m_targetType(TEXT_TARGET_TYPE::END),
	m_usage(TEXT_USAGE::END),
	m_textPos(Vector2(0.f, 0.f)),
	m_fontSize(10.f),
	m_redColor(200),
	m_greenColor(200),
	m_blueColor(200),
	m_alphaColor(255),
	m_damage(0),
	m_bShow(true)
{
	m_fontColor = FONT_RGBA(m_redColor, m_greenColor, m_blueColor, m_alphaColor);
}

TextUIScript::~TextUIScript()
{
}

void TextUIScript::start()
{
	if (TEXT_TARGET_TYPE::MONSTER == m_targetType)
	{
		m_bShow = false;
	}

	Vector3 pos = GetTransform()->GetLocalPos();
	pos.z = 10.f;
	GetTransform()->SetLocalPos(pos);
}

void TextUIScript::update()
{
}

void TextUIScript::lateUpdate()
{
	GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");
	const vector<Script*>& vectorScript = camera->GetScripts();

	for (size_t i = 0; i < vectorScript.size(); ++i)
	{
		if (vectorScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::CAMERASCRIPT)
		{
			if (((CameraScript*)vectorScript[i])->IsEnter())
			{
				m_bShow = false;
			}
			else
			{
				if (TEXT_TARGET_TYPE::PLAYER == m_targetType)
				{
					m_bShow = true;
				}

				if (TEXT_TARGET_TYPE::ITEM == m_targetType)
				{
					GameObject* torch = SceneManager::GetInst()->FindObjByName(L"Item_Torch");
					if (((UIScript*)torch->GetScripts()[0])->GetCurTargetUI())
					{
						m_bShow = true;
					}
					else
					{
						m_bShow = false;
					}
				}
			}
		}
	}
}

void TextUIScript::render()
{
	if(m_bShow)
		FontManager::GetInst()->DrawFont(m_text.c_str(), m_textPos.x, m_textPos.y, m_fontSize, m_fontColor);
}

void TextUIScript::SaveToScene(FILE* _file)
{
	SaveWStringToFile(m_text, _file);
	SaveToFile(&m_targetType, _file);
	SaveToFile(&m_usage, _file);
	SaveToFile(&m_textPos, _file);
	SaveToFile(&m_fontSize, _file);
	SaveToFile(&m_fontColor, _file);
}

void TextUIScript::LoadFromScene(FILE* _file)
{
	LoadWStringFromFile(m_text, _file);
	LoadFromFile(&m_targetType, _file);
	LoadFromFile(&m_usage, _file);
	LoadFromFile(&m_textPos, _file);
	LoadFromFile(&m_fontSize, _file);
	LoadFromFile(&m_fontColor, _file);
}
