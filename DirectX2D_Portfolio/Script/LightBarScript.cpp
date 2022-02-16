#include "pch.h"
#include "LightBarScript.h"
#include "LightScript.h"
#include <Engine/Transform.h>
#include <Engine/GameObject.h>
#include <Engine/SceneManager.h>

LightBarScript::LightBarScript() :
	UIScript(SCRIPT_TYPE::LIGHTBARSCRIPT),
	m_originXScaleRate(0.f)
{
}

LightBarScript::~LightBarScript()
{
}

void LightBarScript::start()
{
	UIScript::start();

	Vector3 scale = GetTransform()->GetLocalScale();
	Vector3 parentScale = Object()->GetParent()->GetTransform()->GetLocalScale();

	m_originXScaleRate = scale.x / parentScale.x;
}

void LightBarScript::update()
{
	UIScript::update();

	Vector3 parentScale = Object()->GetParent()->GetTransform()->GetLocalScale();
	Vector3 scale = GetTransform()->GetLocalScale();
	GameObject* light = SceneManager::GetInst()->FindObjByName(L"Light");
	const vector<Script*>& vectorScript = light->GetScripts();
	float ratio = 0.f;

	for (size_t i = 0; i < vectorScript.size(); ++i)
	{
		if ((UINT)SCRIPT_TYPE::LIGHTSCRIPT == vectorScript[i]->GetScriptType())
		{
			ratio = ((LightScript*)vectorScript[i])->GetLightRatio();
		}
	}

	scale.x = m_originXScaleRate * ratio;

	GetTransform()->SetLocalScale(scale);
}

void LightBarScript::lateUpdate()
{
	UIScript::lateUpdate();
}

void LightBarScript::SaveToScene(FILE* _file)
{
}

void LightBarScript::LoadFromScene(FILE* _file)
{
}
